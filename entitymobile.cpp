#include "entitymobile.hpp"
#include <iostream>

EntityMobile::EntityMobile() :
	Entity()
{
	life 	= 0.0f;
	maxLife	= 0.0f;
	defense	= 0.0f;
	isDead	= false;
}

EntityMobile::EntityMobile(const float& plife, const float& pMaxLife, const float& pdefense) :
	Entity(), life(plife), maxLife(pMaxLife), defense(pdefense), isDead(false)
{
}

EntityMobile::~EntityMobile()
{
}
