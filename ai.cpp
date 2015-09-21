#include "main.hpp"
#include <math.h>

/**
 * Constructeur
 */
Ai::Ai(Object* powner) :
	owner(powner)
{
}

/******************************************************************************
 *                                  MONSTERAI                                 *
 ******************************************************************************/
/**
 * Constructeur
 */
MonsterAi::MonsterAi(Object* powner) :
	Ai(powner), moveSpeed(0)
{
}

/**
 * Mise à jour du monstre : déplacement ou combat
 */
void MonsterAi::update()
{
	if (owner->destructible && owner->destructible->isDead())
		return;
	
	if (Engine::getInstance()->getMap()->isInFov(owner->posX, owner->posY))
	{
		//le monstre est dans le FOV du joueur
		//le monstre VOIT le joueur
		//si le monstre perd de vue le joueur, il a "TRACKING_TURNS" de tours pour
		//avancer vers le dernier point vu par le joueur
		moveSpeed = TRACKING_TURNS;
	}
	else
	{
		//le monstre NE VOIT pas le joueur
		//On décrémente son nombre de possibilité de retrouver le joueur
		moveSpeed--;
	}
	
	//déplacement du monstre vers le joueur si il reste des tours possibles pour le monstre
	if (moveSpeed > 0)
		moveOrAttack(Engine::getInstance()->getPlayer()->posX, Engine::getInstance()->getPlayer()->posY);
		
}

/**
 * @brief Déplacement ou attaque du monstre
 * @param powner Pointeur vers le monstre
 * @param ptargetX Position de la cible
 * @param ptargetY Position de la cible
 */
void MonsterAi::moveOrAttack(const int& ptargetX, const int& ptargetY)
{
	//delta entre le joueur et le monstre
	int deltaX 		= Engine::getInstance()->getPlayer()->posX - owner->posX;
	int deltaY 		= Engine::getInstance()->getPlayer()->posY - owner->posY;
	//déplacement positif ou négatif ?
	int stepX 		= (deltaX > 0) ? 1 : -1;
	int stepY 		= (deltaY > 0) ? 1 : -1;
	//calcul de la distance entre le joueur et le monstre
	//-> Racine de ( (player X - monstre X)² + (player Y - monstre Y)² )
	float distance	= sqrtf( (deltaX * deltaX) + (deltaY * deltaY) );
	
	if (distance >= 2)
	{
		//on normalise le delta pour avoir l'orientation du déplacement en 1 case
		deltaX = (int)round(deltaX / distance);
		deltaY = (int)round(deltaY / distance);
		
		//
		//déplacement du monstre en direction du joueur
		//
		
		//si la case la plus directe est accessible, déplacement
		if (Engine::getInstance()->getMap()->canWalk(owner->posX + deltaX, owner->posY + deltaY)) {
			owner->posX = owner->posX + deltaX;
			owner->posY = owner->posY + deltaY;
		}
		//sinon on tente de contourner par les X
		else if (Engine::getInstance()->getMap()->canWalk(owner->posX + deltaX, owner->posY)) {
			owner->posX = owner->posX + stepX;
		}
		//sinon on tente de contourner par les Y
		else if (Engine::getInstance()->getMap()->canWalk(owner->posX, owner->posY + deltaY)) {
			owner->posY = owner->posY + stepY;
		}
	}
	else if (owner->attacker)
	{
		//le monstre attaque le joueur
		owner->attacker->attack(Engine::getInstance()->getPlayer());
	}
	
	//Engine::getInstance()->getGui()->message(TCODColor::white, "distance entre M et P : %f", distance);
}


/******************************************************************************
 *                                   PLAYERAI                                 *
 ******************************************************************************/
PlayerAi::PlayerAi(Object* powner):
	Ai(powner)
{
}

void PlayerAi::update()
{
	//le joueur est mort
	if (owner->destructible && owner->destructible->isDead())
		return;
		
	int nx = 0;
	int ny = 0;
	
    switch(Engine::getInstance()->key.vk)
	{
        case TCODK_UP 		: ny = -1; break;
        case TCODK_DOWN 	: ny =  1; break;
        case TCODK_LEFT 	: nx = -1; break;
        case TCODK_RIGHT 	: nx =  1; break;
		
		case TCODK_CHAR		: processKey(Engine::getInstance()->key.c); break;
		
		case TCODK_DELETE	: Engine::getInstance()->getGui()->clear(); break;
        default: break;
	}
	
	//un déplacement est lancé
	//mise à jour de la position du joueur
	//traitement du fov
	if (nx != 0 || ny != 0)
	{
		Engine::getInstance()->setGameStatus(NEW_TURN);
		
		//std::cout << "position joueur : " << owner->posX << ";" << owner->posY << std::endl;
		//std::cout << "nx, ny : " << owner->posX + nx << ";" << owner->posY + ny << std::endl;
		
		if (moveOrAttack(owner->posX + nx, owner->posY + ny))
			Engine::getInstance()->getMap()->computeFov();
	}
}

bool PlayerAi::moveOrAttack(const int& ptargetX, const int& ptargetY)
{
	//la case cible est un mur
	if (Engine::getInstance()->getMap()->isWall(ptargetX, ptargetY))
		return false;
	
	//il y a un object dans la case ciblé
	for (Object **it = Engine::getInstance()->getObjects().begin(); 
			it != Engine::getInstance()->getObjects().end(); it++)
	{
		Object* obj = *it;
		
		if (obj->posX != ptargetX || obj->posY != ptargetY)
			continue;

			if (obj->destructible && !obj->destructible->isDead())
			{
				//
				//objet vivant
				//
				Engine::getInstance()->getGui()->message(TCODColor::orange, "Attaque !!!");
				owner->attacker->attack(obj);
				return false;
			} 
			else if (obj->destructible && obj->destructible->isDead())
			{
				//
				//objet mort
				//
				Engine::getInstance()->getGui()->message(TCODColor::orange, "Le corps de %s", obj->name);
			}
			else if (obj->usable)
			{
				//
				//item
				//
				Engine::getInstance()->getGui()->message(TCODColor::blue, "%s est au sol", obj->name);
			}
			
			//l'objet est à la position ciblé par le joueur
			//et il est de type attacker
			
			//std::cout << "obj : " << obj->posX << ";" << obj->posY << std::endl;
			//std::cout << "local : " << x << ";" << y << std::endl;
			
			//Engine::getInstance()->getGui()->message(TCODColor::red, "Rencontre avec %s", obj->name);
	}
	
	//déplacement effectué, mise à jour de la position du joueur
	owner->posX = ptargetX;
	owner->posY = ptargetY;
	
	return true;
}

void PlayerAi::processKey(const int& key)
{
	switch(key)
	{
		//
		//le joueur ramasse un objet au sol
		//
		case 'g': {
			for (Object **it = Engine::getInstance()->getObjects().begin(); it != Engine::getInstance()->getObjects().end(); it++)
			{
				Object* obj = *it;
				
				if (obj->isInPosition(owner->posX, owner->posY))
				{
					takeItem(obj);
					break;
				}
			}
			break;
		}
		//
		//afficher l'inventaire et les informations des objets
		//
		case 'i': showInventary(); break;
		//
		//utilisation d'un objet depuis l'inventaire
		//
		case 'a': {
			Object *item = choseFromInventary();
			if (item) {
				item->usable->use(owner);
				Engine::getInstance()->setGameStatus(NEW_TURN);
			}
			break;
		}
		//
		//drop d'un objet
		//
		case 'd': {
			Object* item = choseFromInventary();
			if (item) {
				item->usable->drop(owner);
				Engine::getInstance()->setGameStatus(NEW_TURN);	
			}
			break;
		}
		//
		// DEV Test
		//
		case 'p': {
			int x = 0;
			int y = 0;
			Engine::getInstance()->pickTile(x, y, 100.0f);
			std::cout << "x, y : " << x << " - " << y << std::endl;
			break;
		}
	}
}

/**
 * Tente de récupérer l'objet si l'inventaire n'est pas complet et affiche un message
 * @param powner Le joueur
 * @param item L'objet à ramasser
 */
bool PlayerAi::takeItem(Object* item)
{
	//nouveau tour
	Engine::getInstance()->setGameStatus(NEW_TURN);
	
	if (!item->usable)
		return false;
		
	if (item->usable->pick(owner)) {
		Engine::getInstance()->getGui()->message(TCODColor::azure, "Vous avez récupéré %", item->name);
		return true;
	} else {
		Engine::getInstance()->getGui()->message(TCODColor::azure, "Votre inventaire est complet");
		return false;
	}
	return false;
}

/**
 * Affiche l'inventaire et retourne l'objet choisi si il existe sinon NULL
 * @param powner Le joueur
 */
Object* PlayerAi::choseFromInventary()
{
	static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Utiliser");
	console.setDefaultForeground(TCODColor::white);
	
	int shortcut 	= 'a';
	int posY 		= 1;
	
	for (Object **it = owner->container->getInventory().begin(); it != owner->container->getInventory().end(); it++)
	{
		Object* item = *it;
		
		console.print(2, posY, "%c - %s", shortcut, item->name);
		
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
		
		//std::cout << "inv size : " << owner->container->getInventory().size() << std::endl;
		//std::cout << "index : " << objIndex << std::endl;
	
		if (objIndex >= 0 && objIndex < owner->container->getInventory().size())
			return owner->container->getInventory().get(objIndex);
	}
	
	return NULL;
}

/**
 * Affiche l'inventaire et la description de l'objet sélectionné
 * @param powner Le joueur
 */
void PlayerAi::showInventary()
{
	static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Inventaire");
	console.setDefaultForeground(TCODColor::white);
	
	while (!TCODConsole::isWindowClosed())
	{
		int shortcut 	= 'a';
		int posY 		= 1;
		
		for (Object **it = owner->container->getInventory().begin(); it != owner->container->getInventory().end(); it++)
		{
			Object* item = *it;
			
			console.print(2, posY, "%c - %s", shortcut, item->name);
			
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
		
		if (key.vk == TCODK_ESCAPE)
			return;
		
		if (key.vk == TCODK_CHAR) {
			int objIndex = key.c - 'a';
			
			//std::cout << "inv size : " << owner->container->getInventory().size() << std::endl;
			//std::cout << "index : " << objIndex << std::endl;
		
			if (objIndex >= 0 && objIndex < owner->container->getInventory().size())
			{
				//afficher les informations de l'objet sur la partie de droite
				Object* obj = owner->container->getInventory().get(objIndex);
				console.print(20, 4, obj->description);
				
				std::cout << "obj desc : " << obj->description << std::endl;
			}
		}
		TCODConsole::flush();
	}
}

/**
 * Affiche l'écran du personnage
 */
void PlayerAi::showEquipment()
{
}
