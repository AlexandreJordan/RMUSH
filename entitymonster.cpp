#include "entitymonster.hpp"
#include "engine.hpp"
#include <math.h>

EntityMonster::EntityMonster() :
	EntityMobile()
{
}

EntityMonster::EntityMonster(const float& plife, const float& pMaxLife, const float& pdefense, const float& pfov) :
	EntityMobile(plife, pMaxLife, pdefense, pfov)
{
}

EntityMonster::~EntityMonster()
{
}

/**
 * Dessin du PNJ
 */
void EntityMonster::render()
{
	EntityMobile::render();
	
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

/**
 * Mise à jour du PNJ
 * Déplacement du pnj vers le joueur
 */
void EntityMonster::update()
{
	if (isDead)
		return;
	
	if (Engine::getInstance()->getMap().getCurrentLevel().isInFov(x, y))
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
		moveOrAttack(Engine::getInstance()->getPlayer().x, Engine::getInstance()->getPlayer().y);
}

/**
 * @brief Déplacement ou attaque du monstre
 * @param powner Pointeur vers le monstre
 * @param ptargetX Position de la cible
 * @param ptargetY Position de la cible
 */
void EntityMonster::moveOrAttack(const int& ptargetX, const int& ptargetY)
{
	//delta entre le joueur et le monstre
	int deltaX 		= Engine::getInstance()->getPlayer().x - x;
	int deltaY 		= Engine::getInstance()->getPlayer().y - y;
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
		if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x + deltaX, y + deltaY)) {
			x = x + deltaX;
			y = y + deltaY;
		}
		//sinon on tente de contourner par les X
		else if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x + deltaX, y)) {
			x = x + stepX;
		}
		//sinon on tente de contourner par les Y
		else if (Engine::getInstance()->getMap().getCurrentLevel().canWalk(x, y + deltaY)) {
			y = y + stepY;
		}
	}
	else
	{
		//le monstre attaque le joueur
		//TODO attaque du joueur
		Engine::getInstance()->getGui().message(TCODColor::red, "%s vous attaque", name.c_str());
	}
}
