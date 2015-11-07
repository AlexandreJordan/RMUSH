#ifndef LEVER_HPP
#define LEVER_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"
#include <iostream>

class Lever : public EntityFixedItem
{
public:
    Lever();
    ~Lever();

	void render();
	void update();

    void use(EntityMobile* powner);

	EntityFixedItem* ItemLink;
};

#endif

/**
 * Constructeur
 */
Lever::Lever() :
	EntityFixedItem()
{
}

/**
 * Destructeur
 */
Lever::~Lever()
{
}

/**
 * Utilisation de l'item directement au sol
 */
void Lever::use(EntityMobile* powner)
{
	if (!ItemLink)
		Engine::getInstance()->getGui().message(TCODColor::green, "Le levier n'est relié à rien...");
	
	Engine::getInstance()->getGui().message(TCODColor::green, "Vous utilisez le levier.");
	ItemLink->use(powner);
}

void Lever::update()
{
}

void Lever::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

