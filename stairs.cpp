#include "stairs.hpp"
#include "engine.hpp"

/**
 * Constructeur
 */
StairUp::StairUp() :
	EntityFixedItem()
{
}

/**
 * Destructeur
 */
StairUp::~StairUp()
{
}

/**
 * Utilisation de l'item directement au sol
 */
void StairUp::use(EntityMobile* powner)
{
	Engine::getInstance()->getGui().message(TCODColor::green, "Vous montez à l'étage supérieur.");
	Engine::getInstance()->getMap().changeLevelToUp();
}

void StairUp::update()
{
}

void StairUp::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

///////////////////////////////////////////////

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