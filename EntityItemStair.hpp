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

    void use(EntityMobile* powner, EntityMobile* ptarget);

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
void EntityItemBandage::use(EntityMobile* powner, EntityMobile* ptarget)
{
    std::cout << powner->name << " utilise " << this->name << " sur " << ptarget->name << std::endl;
}

void EntityItemBandage::update()
{
}

void EntityItemBandage::render()
{
	TCODConsole::root->setChar(x, y, chr);
	TCODConsole::root->setCharForeground(x, y, color);
}

