#ifndef ENTITYITEMBANDAGE_HPP
#define ENTITYITEMBANDAGE_HPP

#include "entityitem.hpp"
#include "entitymobile.hpp"

class EntityItemBandage : public EntityItem
{
public:
    EntityItemBandage();
    ~EntityItemBandage();

	void render();
	void update();

    void use(EntityMobile* powner);
	void useFromGround(EntityMobile* powner);

	int lifeRestore;
};

#endif

/**
 * Constructeur
 */
EntityItemBandage::EntityItemBandage() :
	EntityItem()
{
	lifeRestore = 0;
}

/**
 * Destructeur
 */
EntityItemBandage::~EntityItemBandage()
{
}


/**
 * Utilisation de l'item
 */
void EntityItemBandage::use(EntityMobile* powner)
{
    std::cout << powner->name << " utilise " << this->name << std::endl;
}

/**
 * Utilisation de l'item directement au sol
 */
void EntityItemBandage::useFromGround(EntityMobile* powner)
{
}

void EntityItemBandage::update()
{
}

void EntityItemBandage::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

