#include "entitymobile.hpp"
#include "engine.hpp"
#include "tools.hpp"
#include <iostream>

EntityMobile::EntityMobile() :
	Entity()
{
	life 		= 0.0f;
	maxLife		= 0.0f;
	defense		= 0.0f;
	fov			= 0.0f;
	showFov		= false;
	isDead		= false;
}

EntityMobile::EntityMobile(const float& plife, const float& pmaxLife, const float& pdefense, const float& pfov) :
	Entity(), life(plife), maxLife(pmaxLife), defense(pdefense), isDead(false), fov(pfov)
{
}

EntityMobile::~EntityMobile()
{
}

void EntityMobile::render()
{
	if (!showFov)
		return;
	
	for (int nx = x - fov; nx <= x + fov; nx++)
		for (int ny = y - fov; ny <= y + fov; ny++)
		{
			if (Tools::getDistance(x ,y , nx, ny) <= fov && !Engine::getInstance()->getMap().getCurrentLevel().isWall(nx, ny)) {
				TCODColor col = TCODConsole::root->getCharBackground(nx, ny);
				col = col * 0.5f;
				TCODConsole::root->setCharBackground(nx, ny, col);
			}
		}
}

void EntityMobile::rtupdate()
{
}
