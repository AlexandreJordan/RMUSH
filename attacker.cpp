#include "main.hpp"

Attacker::Attacker(Object* powner, const float& ppower) :
	power(ppower), owner(powner)
{
}

void Attacker::attack(Object* ptarget)
{
	Engine::getInstance()->getGui()->message(TCODColor::green, "Attaque de %s sur %s", owner->name, ptarget->name);
	ptarget->destructible->takeDamage(owner->attacker->power);
}