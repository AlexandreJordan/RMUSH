#ifndef STAIRDOWN_HPP
#define STAIRDOWN_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"
#include <iostream>

class StairDown : public EntityFixedItem
{
public:
    StairDown();
    ~StairDown();

	void render();
	void update();

    void use(EntityMobile* powner);
};

#endif

/**
 * Constructeur
 */
StairDown::StairDown() :
	EntityFixedItem()
{
}

/**
 * Destructeur
 */
StairDown::~StairDown()
{
}

/**
 * Utilisation de l'item directement au sol
 */
void StairDown::use(EntityMobile* powner)
{
	Engine::getInstance()->getGui().message(TCODColor::green, "Vous descendez à l'étage inférieur.");
	Engine::getInstance()->getMap().changeLevelToDown();
}

void StairDown::update()
{
}

void StairDown::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

