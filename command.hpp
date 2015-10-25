#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"
#include <iostream>

class Command : public EntityFixedItem
{
public:
    Command();
    ~Command();

	void render();
	void update();

    void use(EntityMobile* powner);
	void useFromGround(EntityMobile* powner);
};

#endif

/**
 * Constructeur
 */
Command::Command() :
	EntityFixedItem()
{
}

/**
 * Destructeur
 */
Command::~Command()
{
}


/**
 * Utilisation de l'item
 */
void Command::use(EntityMobile* powner)
{}

/**
 * Utilisation de l'item directement au sol
 */
void Command::useFromGround(EntityMobile* powner)
{
	//Engine::getInstance()->getGui().message(TCODColor::green, "Vous montez à l'étage supérieur.");
}

void Command::update()
{
}

void Command::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

