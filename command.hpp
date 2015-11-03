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

	EntityFixedItem* ItemLink;
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
 * Utilisation de l'item directement au sol
 */
void Command::use(EntityMobile* powner)
{
	if (!ItemLink)
		Engine::getInstance()->getGui().message(TCODColor::green, "Le levier n'est relié à rien...");
	
	Engine::getInstance()->getGui().message(TCODColor::green, "Vous utilisez le levier.");
	ItemLink->use(powner);
}

void Command::update()
{
}

void Command::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

