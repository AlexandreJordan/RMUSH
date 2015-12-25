#ifndef REMOTECONTROL_HPP
#define REMOTECONTROL_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"
#include <iostream>

class RemoteControl : public EntityItem
{
public:
    RemoteControl();
    ~RemoteControl();

	void render();
	void update();

    void use(EntityMobile* powner);

	EntityItem* ItemLink;
};

#endif

/**
 * Constructeur
 */
RemoteControl::RemoteControl() :
	EntityItem()
{
}

/**
 * Destructeur
 */
RemoteControl::~RemoteControl()
{
}

/**
 * Utilisation de l'item directement au sol
 */
void RemoteControl::use(EntityMobile* powner)
{
	if (!ItemLink)
		Engine::getInstance()->getGui().message(TCODColor::green, "La télécommande n'est relié à rien...");
	
	Engine::getInstance()->getGui().message(TCODColor::green, "Vous utilisez la télécommande.");
	ItemLink->use(powner);
}

void RemoteControl::update()
{
}

void RemoteControl::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

