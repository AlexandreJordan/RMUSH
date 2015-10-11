#include "entityplayer.hpp"
#include "engine.hpp"
#include <iostream>

EntityPlayer::EntityPlayer() :
	EntityMobile()
{
}

EntityPlayer::EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense) :
	EntityMobile(plife, pMaxLife, pdefense)
{
}

EntityPlayer::~EntityPlayer()
{
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
			Engine::getInstance()->getMap().getCurrentLevel().computeFov();
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
		case 'i' : std::cout << "appel inventaire" << std::endl; break;
		case 'd' : std::cout << "appel drop" << std::endl; break;
		case 'g' : std::cout << "appel get" << std::endl; break;
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
	for (EntityMonster **it = lvl.getMonstersList().begin(); it != lvl.getMonstersList().end(); it++)
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
			//TODO attaque !
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
		Engine::getInstance()->getGui().message(TCODColor::orange, "%s est au sol", item->name.c_str());
	}
		
	//déplacement effectué, mise à jour de la position du joueur
	x = ptargetX;
	y = ptargetY;
	
	return true;
}
