#include "entityplayer.hpp"
#include "engine.hpp"
#include "tools.hpp"
#include "stairs.hpp"
#include <iostream>
#include <string>

using namespace std;

EntityPlayer::EntityPlayer() :
	EntityMobile()
{
	inventorySize_ = 26;
}

EntityPlayer::EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense, const float& pfov) :
	EntityMobile(plife, pMaxLife, pdefense, pfov)
{
}

EntityPlayer::~EntityPlayer()
{
	inventory_.clearAndDelete();
}

void EntityPlayer::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

void EntityPlayer::update()
{
	int nx = 0;
	int ny = 0;
	
	switch(Engine::getInstance()->getMainKey().vk)
	{
		case TCODK_UP 		: ny = -1; break;
        case TCODK_DOWN 	: ny =  1; break;
        case TCODK_LEFT 	: nx = -1; break;
        case TCODK_RIGHT 	: nx =  1; break;
		case TCODK_CHAR 	: processKey(Engine::getInstance()->getMainKey().c); break;
		default: break;
	}
	
	//un déplacement est lancé
	//mise à jour de la position du joueur
	//traitement du fov
	if (nx != 0 || ny != 0)
	{
		Engine::getInstance()->setMainStatus(NEW_TURN);
		if (moveOrAttack(x + nx, y + ny))
			Engine::getInstance()->getMap().getCurrentLevel().computeFov(Engine::getInstance()->getPlayer().fov);
	}
}

/**
 * Gestion des touches concernant le joueur
 * @param key Touche appuyée par le joueur
 */
void EntityPlayer::processKey(const int& key)
{
	switch(key)
	{
		//utilisation d'un objet de l'inventaire
		case 'a' :
		{
			EntityItem* item = choseOneFromList(inventory_, "Utiliser");
			if (item) item->use(this);
			break;
		}
		
		//utilisation d'un objet au sol
		case 'u' :
		{
			EntityFixedItem* itemAtGround = getFixedItemFromTile(x, y);
			if (itemAtGround) itemAtGround->use(this);
			
			break;
		}
		//monter un escalier
		case '<' :
		{
			EntityFixedItem* itemAtGround = getFixedItemFromTile(x, y);
			StairUp* stair = dynamic_cast<StairUp*>(itemAtGround);
			if (stair) stair->use(this);
			break;
		}
		//descendre un escalier
		case '>' :
		{
			EntityFixedItem* itemAtGround = getFixedItemFromTile(x, y);
			StairDown* stair = dynamic_cast<StairDown*>(itemAtGround);
			if (stair) stair->use(this);
			break;
		}
		//afficher l'inventaire
		case 'i' : std::cout << choseOneFromList(inventory_, "Inventaire") << std::endl; break;
		//lacher un objet
		case 'd' : 
		{
			TCODList<EntityItem*> selectedItems = choseMultiFromList(inventory_);
			for (EntityItem **it = selectedItems.begin(); it != selectedItems.end(); it++)
				dropToGround(*it);
				
			break;
		}
		//récupérer un objet
		case 'g' :
		{
			TCODList<EntityItem*> list = getItemsFromTile(x, y);
			
			if (list.isEmpty()) {
				//
				//pas d'objet au sol
				//
				Engine::getInstance()->getGui().message(C_MESS_INFO, "Il n'y a rien au sol");
			} else if (list.size() == 1) {
				//
				//récupération de l'objet
				//
				if (addToInventory(list.get(0))) {
					list.remove(list.get(0));
					Engine::getInstance()->getGui().message(C_MESS_INFO, "Vous récupérez %s", list.get(0)->name.c_str());
				}
				
			} else if (list.size() > 1) {
				//
				//afficher une boite de dialogue pour le choix multiple
				//
				TCODList<EntityItem*> selectedItems = choseMultiFromList(list);
				for (EntityItem **it = selectedItems.begin(); it != selectedItems.end(); it++)
					addToInventory(*it);
			}
			break;
		}
		//afficher les infos/items d'une case dans le FOV du joueur
		case 'x' :
		{
			int x = 0;
			int y = 0;
			selectTile(x, y, 10.0f);
			
			TCODList<EntityItem*> items = getItemsFromTile(x, y);
			choseOneFromList(items, "Informations");
			
			break;
		}
		
		//DEV se déplacer dans la première pièce
		case 'b' :
		{
			setPosition(Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->x,
						Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->y);
			break;
		}
		//DEV se déplacer dans la première pièce
		case 'n' :
		{
			setPosition(Engine::getInstance()->getMap().getCurrentLevel().getLastRoom()->x,
						Engine::getInstance()->getMap().getCurrentLevel().getLastRoom()->y);
			break;
		}
		default: break;
	}
}

/**
 * Gestion du déplacement demandée. Attaque si un ennemi est sur la case cible ou
 * déplacement si la case est libre
 * @param ptargetX Case cible
 * @param ptargetY Case cible
 */
bool EntityPlayer::moveOrAttack(const int& ptargetX, const int& ptargetY)
{
	//récupération du level en cours
	Level& lvl = Engine::getInstance()->getMap().getCurrentLevel();
	
	//la case cible est un mur
	if (lvl.isWall(ptargetX, ptargetY))
		return false;
	
	//
	//il y a un PNJ dans la case ciblé
	//
	for (EntityMonster **it = lvl.getPnjsList().begin(); it != lvl.getPnjsList().end(); it++)
	{
		EntityMonster* pnj = *it;
		
		if (pnj->x != ptargetX || pnj->y != ptargetY)
			continue;

		if (!pnj->isDead)
		{
			//
			//objet vivant
			//
			Engine::getInstance()->getGui().message(TCODColor::orange, "Vous attaquez %s", pnj->name.c_str());
			return false;
		} 
		else
		{
			//
			//objet mort
			//
			Engine::getInstance()->getGui().message(TCODColor::orange, "Le corps de %s", pnj->name.c_str());
		}
	}
	
	//
	//il y a un item dans la case ciblé
	//
	for (EntityItem **it = lvl.getItemsList().begin(); it != lvl.getItemsList().end(); it++)
	{
		EntityItem* item = *it;

		if (item->x != ptargetX || item->y != ptargetY)
			continue;

		Engine::getInstance()->getGui().message(TCODColor::orange, "%s est au sol", item->name.c_str());
	}

	//
	//il y a un item fixe dans la case ciblé
	//
	for (EntityFixedItem **it = lvl.getFixedItemsList().begin(); it != lvl.getFixedItemsList().end(); it++)
	{
		EntityFixedItem* item = *it;

		if (item->x != ptargetX || item->y != ptargetY)
			continue;

		Engine::getInstance()->getGui().message(TCODColor::orange, "%s", item->name.c_str());
	}
		
	//déplacement effectué, mise à jour de la position du joueur
	x = ptargetX;
	y = ptargetY;
	
	return true;
}

/**
 * Retourne une liste de pointeurs des items d'une case px, py
 */
TCODList<EntityItem*> EntityPlayer::getItemsFromTile(const int& px, const int& py)
{
	Level& lvl = Engine::getInstance()->getMap().getCurrentLevel();
	TCODList<EntityItem*> result;
	
	for (EntityItem **it = lvl.getItemsList().begin(); it != lvl.getItemsList().end(); it++)
	{
		EntityItem* item = *it;
		
		if (item->x == px && item->y == py)
			result.push(item);
	}
	
	return result;
}

/**
 * Retourne l'item fixe présent sur la case px, py
 */
EntityFixedItem* EntityPlayer::getFixedItemFromTile(const int& px, const int& py)
{
	Level& lvl = Engine::getInstance()->getMap().getCurrentLevel();
	for (EntityFixedItem **it = lvl.getFixedItemsList().begin(); it != lvl.getFixedItemsList().end(); it++)
	{
		EntityFixedItem* item = *it;
		if (item->x == px && item->y == py)
			return item;
	}
	
	return NULL;
}

/**
 * Affiche l'inventaire et retourne l'objet choisi si il existe sinon NULL
 * @param powner Le joueur
 */
EntityItem* EntityPlayer::choseOneFromList(TCODList<EntityItem*> plist, const string& title)
{
	static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, title.c_str());
	console.setDefaultForeground(TCODColor::white);
	
	int shortcut 	= 'a';
	int posY 		= 1;
	
	for (EntityItem **it = plist.begin(); it != plist.end(); it++)
	{
		EntityItem* item = *it;
		
		console.print(2, posY, "%c - %s", shortcut, item->name.c_str());
		
		posY++;
		shortcut++;
	}
	
	TCODConsole::blit(&console, 0, 0, 
					  INVENTORY_WIDTH, 
					  INVENTORY_HEIGHT, 
					  TCODConsole::root, 
					  (int)(WINDOW_WIDTH - INVENTORY_WIDTH) / 2, 
					  (int)(WINDOW_HEIGHT - INVENTORY_HEIGHT) / 2);
	TCODConsole::flush();
	
	//attente du choix de l'utilisateur
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	
	//std::cout << "key : " << key.vk << std::endl;
	
	if (key.vk == TCODK_CHAR) {
		int objIndex = key.c - 'a';
	
		if (objIndex >= 0 && objIndex < plist.size())
			return plist.get(objIndex);
	}
	
	return NULL;
}

/**
 * Affiche une boite de dialogue avec la liste des items contenu dans plist
 * 	Retourne les éléments marqué d'un +
 * 	Item non sélectionné 	: -
 * 	Item sélectionné		: +
 */
TCODList<EntityItem*> EntityPlayer::choseMultiFromList(TCODList<EntityItem*> plist)
{
	//création d'une console pour la frame
	static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Drop");
	console.setDefaultForeground(TCODColor::white);
	
	TCOD_key_t key;
	TCODList<EntityItem*> result;
	bool isClosed = false;

	//
	//boucle d'affichage
	//
	while (!isClosed)
	{
		int shortcut 	= 'a';
		char selector	= '-';
		int posY 		= 1;
		
		//affichage de la liste
		for (EntityItem **it = plist.begin(); it != plist.end(); it++)
		{
			EntityItem* item = *it;
			
			selector = ((item->isSelected) ? '+' : '-');
			
			console.print(2, posY, "%c %c %s", shortcut, selector ,item->name.c_str());
			
			posY++;
			shortcut++;
		}

		//gestion de la console
		TCODConsole::blit(&console, 0, 0, 
					  INVENTORY_WIDTH, 
					  INVENTORY_HEIGHT, 
					  TCODConsole::root, 
					  (int)(WINDOW_WIDTH - INVENTORY_WIDTH) / 2, 
					  (int)(WINDOW_HEIGHT - INVENTORY_HEIGHT) / 2);
		TCODConsole::flush();
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

		//sortie de la boucle par ESCAPE
		if (key.vk == TCODK_ESCAPE)
		{
			isClosed = true;
			
			for (EntityItem **it = plist.begin(); it != plist.end(); it++)
				(*it)->isSelected = false;
		}
		
		//ajout / suppression dans la liste des sélectionnés
		if (key.vk == TCODK_CHAR)
		{
			int objIndex = key.c - 'a';
			
			if (objIndex >= 0 && objIndex < plist.size())
			{
				//
				//la touche est un item de la liste
				//
				EntityItem* item = plist.get(objIndex);
				item->isSelected = !item->isSelected;
			}
		}
		
		//validation par ENTER
		if (key.vk == TCODK_ENTER)
		{
			for (EntityItem **it = plist.begin(); it != plist.end(); it++)
				if ((*it)->isSelected) {
					result.push(*it);
					(*it)->isSelected = false;
				}
			
			isClosed = true;
		}
	}
	
	return result;
}

/**
 * Ajoute l'item passé en paramètre dans l'inventaire du joueur
 * Le pointeur est supprimé de la liste des items du niveaux en cours
 * @param item Item ajouté à l'inventaire
 * @return TRUE si l'objet est ajouté (assez de place)
 */
bool EntityPlayer::addToInventory(EntityItem* item)
{
	if (inventory_.size() > inventorySize_)
		return false;
		
	inventory_.push(item);
	Engine::getInstance()->getMap().getCurrentLevel().getItemsList().remove(item);
	return true;		
}

/**
 * Dépose item au sol et le retire de l'inventaire
 * @param item Item à déposer au sol
 * @return TRUE si l'objet est déposé au sol
 */
void EntityPlayer::dropToGround(EntityItem* item)
{
	item->setPosition(x, y);														//mise à jour de la position de l'item
	Engine::getInstance()->getMap().getCurrentLevel().getItemsList().push(item);	//ajoute l'item dans la liste du level
	inventory_.remove(item);														//retire l'item de l'inventaire
}

/**
 * Sélection d'une case par le joueur
 */
void EntityPlayer::selectTile(int& px, int& py, const float& prange)
{
	TCOD_key_t key;
	int lastX 		= x;
	int lastY 		= y;
	bool isClosed 	= false;
	
	Level& currentLevel = Engine::getInstance()->getMap().getCurrentLevel();
	
	//
	//boucle d'affichage
	//
	while (!isClosed)
	{
		Engine::getInstance()->render();
		
		//mettre en avant les case qui SONT dans le FOV et dans le RANGE (portée donnée)
		for (int mx = 0; mx < currentLevel.getWidth(); mx++)
			for (int my = 0; my < currentLevel.getHeight(); my++)
			{
				if (currentLevel.isInFov(mx, my) && !currentLevel.isWall(mx, my) && (prange >= 0 || Tools::getDistance(x, y, mx, my) <= prange))
				{
					TCODColor col = TCODConsole::root->getCharBackground(mx, my);
					col = col * 1.1f;
					TCODConsole::root->setCharBackground(mx, my, col);
				}
			}
		
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
		
		switch (key.vk)
		{
			case TCODK_LEFT : {
				lastX -= 1;
				break;
			}
			case TCODK_RIGHT : {
				lastX += 1;
				break;
			}
			case TCODK_UP : {
				lastY -= 1;
				break;
			}
			case TCODK_DOWN : {
				lastY += 1;
				break;
			}
			case TCODK_ENTER : {
				px = lastX;
				py = lastY;
			}
				
			case TCODK_ESCAPE :
				isClosed = true;
				
			default :
				isClosed = true;
		}

		//position du curseur
		TCODConsole::root->setCharBackground(lastX, lastY, TCODColor::white);
		
		TCODConsole::flush();
	}
}
