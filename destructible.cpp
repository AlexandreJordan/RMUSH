#include "main.hpp"

/******************************************************************************
 *                                 DESTRUCTIBLE                               *
 ******************************************************************************/
Destructible::Destructible(Object* powner, const float& php, const float& pmaxHp) :
	maxHp(pmaxHp), hp(php), owner(powner)
{
}

float Destructible::takeDamage(const float& pdamage)
{
	Engine::getInstance()->getGui()->message(TCODColor::green, "%s prend %i de dommage", owner->name, (int)pdamage);
	hp -= pdamage;
	
	if (hp <= 0)
		die();
		
	return hp;
}

void Destructible::die()
{
	owner->chr		= '%';
	owner->color	= TCODColor::darkRed;
	owner->block	= false;
}

float Destructible::heal(const float& plife)
{
	float amount = plife;
	hp += amount;
	
	if (hp > maxHp) {
		amount -= hp - maxHp;
		hp = maxHp;
	}
	
	return amount;
}

/******************************************************************************
 *                            MONSTERDESTRUCTIBLE                             *
 ******************************************************************************/
MonsterDestructible::MonsterDestructible(Object* powner, const float& php, const float& pmaxHp) :
	Destructible(powner, php, pmaxHp)
{
}

void MonsterDestructible::die()
{
	Engine::getInstance()->getGui()->message(TCODColor::red, "Le monstre %s est mort", owner->name);
	Destructible::die();
	Engine::getInstance()->insertObjectStartList(owner);
}


/******************************************************************************
 *                            PLAYERDESTRUCTIBLE                              *
 ******************************************************************************/
PlayerDestructible::PlayerDestructible(Object* powner, const float& php, const float& pmaxHp) :
	Destructible(powner, php, pmaxHp)
{
}

void PlayerDestructible::die()
{
	Engine::getInstance()->getGui()->message(TCODColor::red, "Vous êtes mort !");
	Destructible::die();
	Engine::getInstance()->setGameStatus(LOSE);
}
