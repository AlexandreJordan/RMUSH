#include "entity.hpp"

Entity::Entity()
{
	x 				= 0;
	y 				= 0;
	chr 			= 0;
	description 	= "";
	block 			= false;
	color 			= TCODColor::white;
}

Entity::~Entity()
{
}
