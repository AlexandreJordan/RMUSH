#ifndef GAZTRAP_HPP
#define GAZTRAP_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"
#include <iostream>

class GazTrap : public EntityFixedItem
{
public:
    GazTrap();
    ~GazTrap();

	void render();
	void update();

    void use(EntityMobile* powner);
};

#endif

/**
 * Constructeur
 */
GazTrap::GazTrap() :
	EntityFixedItem()
{
}

/**
 * Destructeur
 */
GazTrap::~GazTrap()
{
}

/**
 * Utilisation de l'item directement au sol
 */
void GazTrap::use(EntityMobile* powner)
{
	Engine::getInstance()->getGui().message(TCODColor::green, "Un nuage de gaz s'echappe d'une trappe.");
}

void GazTrap::update()
{
}

void GazTrap::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

