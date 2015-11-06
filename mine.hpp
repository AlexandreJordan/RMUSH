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

	int rangeDestruction;
};

#endif

/**
 * Constructeur
 */
Mine::Mine() :
	EntityItem()
{
	rangeDestruction = 0;
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
    std::cout << powner->name << " utilise " << this->name << std::endl;
}

void Mine::update()
{
}

void Mine::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

