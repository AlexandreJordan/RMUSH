#ifndef EntityMobile_HPP
#define EntityMobile_HPP

#include "entity.hpp"

class EntityMobile : public Entity
{
public:
	EntityMobile();
    EntityMobile(const float& plife, const float& pMaxLife, const float& pdefense);
    ~EntityMobile();

	void render();

	float life;
	float maxLife;
	float defense;
	
	bool isDead;
	int moveSpeed;
};

#endif
