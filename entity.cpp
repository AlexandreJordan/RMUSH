#include "entity.hpp"

/**
 * Constructeur 
 */
Entity::Entity()
{
	x 				= 0;
	y 				= 0;
	chr 			= 0;
	description 	= "";
	block 			= false;
	color 			= TCODColor::white;
}

/**
 * Destructeur 
 */
Entity::~Entity()
{
}

/**
 * Modifie la position de l'entité
 * @param px Nouvelle position x
 * @param py Nouvelle position y
 */
void Entity::setPosition(const int& px, const int& py)
{
	x = px;
	y = py;
}
