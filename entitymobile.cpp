#include "entitymobile.hpp"
#include <iostream>

EntityMobile::EntityMobile() :
	Entity()
{
	life 		= 0.0f;
	maxLife		= 0.0f;
	defense		= 0.0f;
	rangeView	= 0.0f;
	isDead		= false;
}

EntityMobile::EntityMobile(const float& plife, const float& pmaxLife, const float& pdefense, const float& prangeView) :
	Entity(), life(plife), maxLife(pmaxLife), defense(pdefense), isDead(false), rangeView(prangeView)
{
}

EntityMobile::~EntityMobile()
{
}
