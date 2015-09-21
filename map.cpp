#include "map.hpp"

class BspListener : public ITCODBspCallback
{
private:
	Map &map;
	int lastRoomCenterX;
	int lastRoomCenterY;
	int roomCount;
	
public:
	BspListener(Map &m) : map(m), roomCount(0) {}
	
	bool visitNode(TCODBsp* node, void* userData)
	{
		//std::cout << "roomCount : " << roomCount << std::endl;
		
		if (node->isLeaf())
		{
			int width, height, posX, posY;
		
			//paramètre de la nouvelle pièce
			width	= map.rnd->getInt(ROOM_MIN_SIZE, node->w - 2);
			height	= map.rnd->getInt(ROOM_MIN_SIZE, node->h - 2);
			posX	= map.rnd->getInt(node->x + 1, node->x + node->w - width - 1);
			posY	= map.rnd->getInt(node->y + 1, node->y + node->h - height - 1);
			
			/*std::cout << "Création de la piece" << std::endl;
			std::cout << "width : " << width << std::endl;
			std::cout << "height : " << height << std::endl;
			std::cout << "posX : " << posX << std::endl;
			std::cout << "posY : " << posY << std::endl;*/
			
			//création de la pièce
			map.createRoom((roomCount == 0), posX, posY, posX + width - 1, posY + height - 1);
			
			//création d'un couloir vers la dernière si ce n'est pas la première pièce
			if (roomCount != 0) {
				map.dig(lastRoomCenterX, lastRoomCenterY, posX + (width / 2), lastRoomCenterY);
				map.dig(lastRoomCenterX, lastRoomCenterY, lastRoomCenterX, posY + (height / 2));
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
 */
Map::Map(const int& pwidth, const int& pheight):
	width(pwidth), height(pheight)
{
	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}

/**
 * Destructeur
 */
Map::~Map()
{
	delete []tiles;
	delete map;
}

/**
 * Initialisation de la carte
 * 
 * Génération d'un nombre aléatoire pour la génération de la carte basé sur un seed
 * création de la carte (tile)
 * création de la carte (fov)
 * création d'un bsp tree pour la génération aléatoire
 * 
 * Accès à une case x,y -> map[x + y * width]
 */
void Map::initialize()
{
	rnd		= new TCODRandom(seed, TCOD_RNG_CMWC);
	tiles 	= new Tile[width * height];
	map 	= new TCODMap(width, height);
}

/**
 * Génère une map avec un BSP
 */
void Map::generateByBSP()
{
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(rnd, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, NULL);
}

/**
 * Rendu de la map
 * 
 * Parcours du tableau (1 dimension)
 * Couleur spécifique si la case est dans le FOV du joueur
 */
void Map::render() const
{
	static const TCODColor darkWall(0,0,100);
    static const TCODColor darkGround(50,50,150);
	static const TCODColor lightWall(130,110,50);
	static const TCODColor lightGround(200,180,50);
	
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (isInFov(x, y)) {
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
			} else if (isExplored(x, y))
			{
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? darkWall : darkGround);
			}
		}
	}
			
}

/**
 * Calcul du FOV suivant la position du joueur et le rayon de vision du joueur
 */
void Map::computeFov()
{
	map->computeFov(Engine::getInstance()->getPlayer()->posX, Engine::getInstance()->getPlayer()->posY, Engine::getInstance()->getFovRadius(), true, FOV_RESTRICTIVE);
}

/**
 * Retourne TRUE si la case est dans le champ de vision du joueur. Information récupéré
 * depuis le TCODMap. Si x,y sont dans le FOV du joueur, mise à jour de l'attribut "explored".
 */
bool Map::isInFov(const int &px, const int &py) const
{
	if (map->isInFov(px, py)) {
		tiles[px + py * width].explored = true;
		return true;
	}
	return false;
}

/**
 * Creuser un rectangle
 * On met à TRUE isTransparent et isWalkable dans le TCODMap
 * 
 * @param x1 Point haut / gauche
 * @param y1 Point haut / gauche
 * @param x2 Point haut / droit
 * @param y2 Point haut / droit
 */
void Map::dig(int px1, int py1, int px2, int py2)
{
	if (px2 < px1)
		Tools::swapInt(px2, px1);
	
	if (py2 < py1)
		Tools::swapInt(py2, py1);
	
	int x, y;
	for (x = px1; x <= px2; x++)
		for (y = py1; y <= py2; y++)
			map->setProperties(x, y, true, true);
}

/**
 * Création d'un pièce
 *
 * @param firt Première pièce créée
 * @param x1 Point haut / gauche
 * @param y1 Point haut / gauche
 * @param x2 Point haut / droit
 * @param y2 Point haut / droit
 */
void Map::createRoom(bool pfirst, int px1, int py1, int px2, int py2)
{
	TCODRandom *rnd = TCODRandom::getInstance();
	dig(px1, py1, px2, py2);
	
	Engine::getInstance()->getStair()->posX = (px2 - px1) / 2;
	Engine::getInstance()->getStair()->posX = (py2 - py1) / 2;
	
	//DEV première pièce, placement du joueur au centre
	if (pfirst)
	{
		Engine::getInstance()->getPlayer()->posX = px1 + (px2 - px1) / 2;
		Engine::getInstance()->getPlayer()->posY = py1 + (py2 - py1) / 2;
	}
	
	//DEV création des monstres - Basique pour le dev
	int nbMonsters = rnd->getInt(0, MAX_ROOM_MONSTERS);
	while(nbMonsters > 0) 
	{
		int x = rnd->getInt(px1, px2);
		int y = rnd->getInt(py1, py2);
		addMonster(x, y);
		nbMonsters--;			
	}
	
	//DEV création des items - Basique pour le dev
	int nbItems = rnd->getInt(0, MAX_ROOM_ITEMS);
	while(nbItems > 0) 
	{
		int x = rnd->getInt(px1, px2);
		int y = rnd->getInt(py1, py2);
		addItem(x, y);
		nbItems--;			
	}
}

/**
 * Retourne TRUE si la case en x,y est un mur
 */
bool Map::isWall(const int &px, const int &py) const
{
	return !map->isWalkable(px, py);
}

/**
 * Retourne True si la case en x,y a déjà été vu par le joueur 
 */
bool Map::isExplored(const int &px, const int &py) const
{
	return tiles[px + py * width].explored;}

/**
 * Retourne TRUE si la case ciblé est vide :
 * 	- Ce n'est pas un mur
 * 	- Il n'y a pas d'Object dessus ayant la propriété "block" à TRUE
 */
bool Map::canWalk(const int &px, const int &py) const
{
	if (isWall(px, py))
		return false;
		
	for (Object **iterator = Engine::getInstance()->getObjects().begin(); 
		 iterator != Engine::getInstance()->getObjects().end(); iterator++)
	{
		//objet de la scène est sur la case visé et est bloquant
		if ((*iterator)->posX == px && (*iterator)->posY == py && (*iterator)->block) {
			//Engine::getInstance()->getGui()->message(TCODColor::green, "collision objet : %i;%i / %b", px, py, (*iterator)->block);
			return false;
		}
	}
	
	return true;
}

/**
 * Révèle toute la carte
 * Passer toutes les tiles en explored à TRUE
 */
void Map::reveal()
{
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			tiles[x + y * width].explored = true;
}

/**
 * Ajoute une monstre dans le donjon
 * @param px Position du monstre
 * @param py Position du monstre
 */
void Map::addMonster(const int &px, const int &py)
{
	Object* monster 		= new Object(px, py, 'o', "monstre", "Un monstre trop fort pour vous", true, TCODColor::yellow);
	monster->destructible 	= new MonsterDestructible(monster, 10, 10);
	monster->attacker		= new Attacker(monster, 2);
	monster->ai				= new MonsterAi(monster);
	
	Engine::getInstance()->getObjects().push(monster);
}

/**
 * Ajoute un item dans le donjon
 * @param px Position de l'item
 * @param py Position de l'item
 */
void Map::addItem(const int& px, const int& py)
{
	TCODRandom* rnd = TCODRandom::getInstance();
	int dice		= rnd->get(0, 100);
	Object* item	= NULL;
	
	if (dice > 70) {
		item = new Object(px, py, '!', "Potion de soin", "La potion de soin restaure de pv.", false, TCODColor::orange);
		item->usable = new HealthPotion(item, 5);
		Engine::getInstance()->getObjects().insertBefore(item, 0);
	} else {
		item = new Object(px, py, 'D', "Arblète", "L'arbalete permet de défoncer du pnj.", false, TCODColor::orange);
		item->usable = new CrossBow(item, 6, 15);
		Engine::getInstance()->getObjects().insertBefore(item, 0);
	}
}
