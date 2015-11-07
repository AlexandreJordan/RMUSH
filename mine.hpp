#ifndef MINE_HPP
#define MINE_HPP

#include "entityitem.hpp"
#include "entitymobile.hpp"

class Mine : public EntityItem
{
public:
    Mine();
    ~Mine();

	void render();
	void update();

    void use(EntityMobile* powner);
	void useFromGround(EntityMobile* powner);

	float rangeDestruction;
	bool startExplosion;
};

#endif

/**
 * Constructeur
 */
Mine::Mine() :
	EntityItem()
{
	rangeDestruction 	= 0.0f;
	startExplosion		= false;
}

/**
 * Destructeur
 */
Mine::~Mine()
{
}


/**
 * Utilisation de l'item
 */
void Mine::use(EntityMobile* powner)
{
    Engine::getInstance()->getGui().message(C_MESS_INFO, "La mine explose !!!");
	startExplosion = true;
}

void Mine::update()
{
	
}

void Mine::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

