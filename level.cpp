#include "level.hpp"
#include "engine.hpp"
#include "entityitembandage.hpp"
#include "stairup.hpp"
#include "stairdown.hpp"
#include "tools.hpp"
#include <fstream>


class BspListener : public ITCODBspCallback
{
private:
	Level &level_;
	int lastRoomCenterX;
	int lastRoomCenterY;
	int roomCount;
	
public:
	BspListener(Level &plevel) : level_(plevel), roomCount(0) {}
	
	bool visitNode(TCODBsp* node, void* userData)
	{
		//std::cout << "roomCount : " << roomCount << std::endl;
		
		if (node->isLeaf())
		{
			int width, height, posX, posY;
		
			//paramètre de la nouvelle pièce
			width	= level_.rnd_->getInt(ROOM_MIN_SIZE, node->w - 2);
			height	= level_.rnd_->getInt(ROOM_MIN_SIZE, node->h - 2);
			posX	= level_.rnd_->getInt(node->x + 1, node->x + node->w - width - 1);
			posY	= level_.rnd_->getInt(node->y + 1, node->y + node->h - height - 1);
			
			/*std::cout << "Création de la piece" << std::endl;
			std::cout << "width : " << width << std::endl;
			std::cout << "height : " << height << std::endl;
			std::cout << "posX : " << posX << std::endl;
			std::cout << "posY : " << posY << std::endl;*/
			
			//création de la pièce
			level_.createRoom((roomCount == 0), posX, posY, posX + width - 1, posY + height - 1);
			
			//création d'un couloir vers la dernière si ce n'est pas la première pièce
			if (roomCount != 0) {
				level_.dig(lastRoomCenterX, lastRoomCenterY, posX + (width / 2), lastRoomCenterY);
				level_.dig(lastRoomCenterX, lastRoomCenterY, lastRoomCenterX, posY + (height / 2));
			}
			
			//on garde le centre de la pièce
			lastRoomCenterX = posX + (width / 2);
			lastRoomCenterY = posY + (height / 2);
			
			roomCount++;
		}
		return true;
	}
};


/**
 * Constructeur
 * @param pwidth Largeur du level
 * @param pheight Hauteur du level
 */
Level::Level(const int& pwidth, const int& pheight) :
	width_(pwidth), height_(pheight)
{
	//pointeur vers le moteur random pour simplifié l'accès
	rnd_ = Engine::getInstance()->randomEngine;
}

/**
 * Destructeur
 * 	Vide et libère les objets des listes
 */
Level::~Level()
{
	rooms_.clearAndDelete();
	monsters_.clearAndDelete();
	items_.clearAndDelete();
	fixedItems_.clearAndDelete();
	
	delete stairUp_;
	delete tcmap_;
	delete [] tiles_;
}

/**
 * Génération du level
 * 	Création du tableau de Tile
 * 	Création de TCODMap
 * 	Ajout des items
 *  Ajout des PNJ
 *  Ajout des escaliers
 */
void Level::generate()
{
	tcmap_	= new TCODMap(width_, height_);
	tiles_ 	= new Tile[width_ * height_];
}

/**
 * Génération des pièces par BSP
 */
void Level::generateBsp()
{
	TCODBsp bsp(0, 0, width_, height_);
	bsp.splitRecursive(rnd_, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
	
	//DEV ajout des escaliers montant / descendant
	createStairUp(rooms_.get(0)->x, rooms_.get(0)->y);
	createStairDown(rooms_.peek()->x, rooms_.peek()->y);
}

/**
 * Génère un niveau depuis un fichier texte
 */
void Level::generateFromFile(const std::string& ppath)
{
	std::ifstream file("../data/levels/test.rmp", std::ios::in);
	
	if (!file)
		return;
		
	std::string line = "";
	while (getline(file, line))
	{
		std::cout << "> " << line << std::endl;
	}
		
	file.close();
}

/**
 * Mise à jour du level
 */
void Level::update()
{
	computeFov();
	
	if (Engine::getInstance()->getMainStatus() == NEW_TURN)
	{
		//mise à jour des PNJ	
		/*for (EntityMonster **iterator = monsters_.begin(); iterator != monsters_.end(); iterator++)
			(*iterator)->update();*/
		
		//mise à jour des items
		for (EntityItem **iterator = items_.begin(); iterator != items_.end(); iterator++)
			(*iterator)->update();
			
		//mise à jour des items fixe
		for (EntityFixedItem **iterator = fixedItems_.begin(); iterator != fixedItems_.end(); iterator++)
			(*iterator)->update();
	}
}

/**
 * Dessin du level
 */
void Level::render()
{
	//dessin du fond de carte
	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			if (isInFov(x, y)) {
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? C_LIGHTWALL : C_LIGHTGROUND);
			} else if (isExplored(x, y))
			{
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? C_DARKWALL : C_DARKGROUND);
			}
		}
	}
	
	//dessins des PNJ	
	for (EntityMonster **iterator = monsters_.begin(); iterator != monsters_.end(); iterator++)
	{
		if (isInFov((*iterator)->x, (*iterator)->y) || Engine::getInstance()->getRevealMode())
			(*iterator)->render();
	}
	
	//dessin des items
	for (EntityItem **iterator = items_.begin(); iterator != items_.end(); iterator++)
	{
		if (isInFov((*iterator)->x, (*iterator)->y) || Engine::getInstance()->getRevealMode())
			(*iterator)->render();
	}
	
	//dessin des items fixes
	for (EntityFixedItem **iterator = fixedItems_.begin(); iterator != fixedItems_.end(); iterator++)
	{
		if (isInFov((*iterator)->x, (*iterator)->y) || Engine::getInstance()->getRevealMode())
			(*iterator)->render();
	}
}

/**
 * Creuse un rectangle
 * On met à TRUE isTransparent et isWalkable dans le TCODMap
 * 
 * @param px1 Point haut / gauche
 * @param py1 Point haut / gauche
 * @param px2 Point haut / droit
 * @param py2 Point haut / droit
 */
void Level::dig(int px1, int py1, int px2, int py2)
{
	if (px2 < px1)
		Tools::swapInt(px2, px1);
	
	if (py2 < py1)
		Tools::swapInt(py2, py1);
	
	int x, y;
	for (x = px1; x <= px2; x++)
		for (y = py1; y <= py2; y++)
			tcmap_->setProperties(x, y, true, true);
}

/**
 * Création d'un pièce
 *
 * @param pfirt Première pièce créée
 * @param px1 Point haut / gauche
 * @param py1 Point haut / gauche
 * @param px2 Point haut / droit
 * @param py2 Point haut / droit
 */
void Level::createRoom(bool pfirst, int px1, int py1, int px2, int py2)
{
	dig(px1, py1, px2, py2);
	
				
	//ajout de la pièce dans la liste
	Room* room = new Room(px1, py1, (px2 - px1), (py2 - py1));
	rooms_.push(room);

	//DEV première pièce, placement du joueur au centre
	if (pfirst)
	{
		Engine::getInstance()->getPlayer().x = px1 + (px2 - px1) / 2;
		Engine::getInstance()->getPlayer().y = py1 + (py2 - py1) / 2;
	}
	
	//DEV création des monstres - Basique pour le dev
	int nbMonsters = rnd_->getInt(0, MAX_ROOM_MONSTERS);
	while(nbMonsters > 0) 
	{
		int x = rnd_->getInt(px1, px2);
		int y = rnd_->getInt(py1, py2);
		addMonster(x, y);
		nbMonsters--;			
	}
	
	//DEV création des items - Basique pour le dev
	int nbItems = rnd_->getInt(0, MAX_ROOM_ITEMS);
	while(nbItems > 0) 
	{
		int x = rnd_->getInt(px1, px2);
		int y = rnd_->getInt(py1, py2);
		addItem(x, y);
		addItem(x, y);
		nbItems--;			
	}
}

/**
 * Calcul du FOV suivant la position du joueur et le rayon de vision du joueur
 */
void Level::computeFov()
{
	tcmap_->computeFov(Engine::getInstance()->getPlayer().x, Engine::getInstance()->getPlayer().y, 10.0f, true, FOV_RESTRICTIVE);
}

/**
 * Retourne TRUE si la case est dans le champ de vision du joueur. Information récupéré
 * depuis le TCODMap. Si x,y sont dans le FOV du joueur, mise à jour de l'attribut "explored".
 */
bool Level::isInFov(const int &px, const int &py) const
{
	if (tcmap_->isInFov(px, py)) {
		tiles_[px + py * width_].explored = true;
		return true;
	}
	return false;
}

/**
 * Retourne TRUE si la case en x,y est un mur
 */
bool Level::isWall(const int &px, const int &py) const
{
	return !tcmap_->isWalkable(px, py);
}

/**
 * Retourne True si la case en x,y a déjà été vu par le joueur 
 */
bool Level::isExplored(const int &px, const int &py) const
{
	return tiles_[px + py * width_].explored;
}

/**
 * Retourne TRUE si la case ciblé est vide :
 * 	- Ce n'est pas un mur
 * 	- Il n'y a pas de pnj dessus
 */
bool Level::canWalk(const int &px, const int &py) const
{
	if (isWall(px, py))
		return false;
		
	/*for (Object **iterator = Engine::getInstance()->getObjects().begin(); 
		 iterator != Engine::getInstance()->getObjects().end(); iterator++)
	{
		//objet de la scène est sur la case visé et est bloquant
		if ((*iterator)->posX == px && (*iterator)->posY == py && (*iterator)->block) {
			//Engine::getInstance()->getGui()->message(TCODColor::green, "collision objet : %i;%i / %b", px, py, (*iterator)->block);
			return false;
		}
	}*/
	
	return true;
}

/**
 * Ajoute un monstre dans le level
 * @param px Position du monstre
 * @param py Position du monstre
 */
void Level::addMonster(const int &px, const int &py)
{
	EntityMonster* monster 	= new EntityMonster();
	monster->block 			= true;
	monster->chr			= 'o';
	monster->color			= C_MONSTER;
	monster->description	= "Un orc pas cool";
	monster->name			= "orc";
	monster->life			= 20;
	monster->defense		= 0;
	monster->x				= px;
	monster->y				= py;
	monsters_.push(monster);
}

/**
 * Ajoute un item dans le level
 * @param px Position de l'item
 * @param py Position de l'item
 */
void Level::addItem(const int& px, const int& py)
{
	int dice = rnd_->get(0, 100);
	
	if (dice > 50)
	{
		EntityItemBandage* item = new EntityItemBandage();
		item->block 			= false;
		item->chr				= 'm';
		item->color				= C_ITEM_BANDAGE;
		item->name				= "Bandage";
		item->description		= "Bandage de soins. Arrête les saignements.";
		item->x					= px;
		item->y					= py;
		item->lifeRestore		= 10;
		items_.push(item);
	}
}

/**
 * Révèle toute la carte
 * Passer toutes les tiles en explored à TRUE
 */
void Level::reveal()
{
	for (int x = 0; x < width_; x++)
		for (int y = 0; y < height_; y++)
			tiles_[x + y * width_].explored = true;
}

/**
 * Créer un EntityStairUp
 */
void Level::createStairUp(const int& px, const int& py)
{
	//TODO utilisation d'un fichier de config pour les caractéristiques de l'objet
	StairUp* item 			= new StairUp();
	item->block 			= false;
	item->chr				= '<';
	item->color				= C_ITEM_STAIR;
	item->name				= "Escalier montant";
	item->description		= "Vous permet de monter à l'étage supérieur";
	item->x					= px;
	item->y					= py;
	fixedItems_.push(item);
}

/**
 * Créer un EntityStairDown
 */
void Level::createStairDown(const int& px, const int& py)
{
	//TODO utilisation d'un fichier de config pour les caractéristiques de l'objet
	StairDown* item 		= new StairDown();
	item->block 			= false;
	item->chr				= '>';
	item->color				= C_ITEM_STAIR;
	item->name				= "Escalier descendant";
	item->description		= "Vous permet de descendre à l'étage inférieur";
	item->x					= px;
	item->y					= py;
	fixedItems_.push(item);
}
