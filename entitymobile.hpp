#ifndef EntityMobile_HPP
#define EntityMobile_HPP

#include "entity.hpp"

class EntityMobile : public Entity
{
public:
	EntityMobile();
    EntityMobile(const float& plife, const float& pmaxLife, const float& pdefense, const float& prangeView);
    ~EntityMobile();

	float life;					//niveau de vie actuel
	float maxLife;				//vie maximale
	float defense;				//point de défense
	
	bool isDead;				//l'entité est morte	
	int moveSpeed;				//vitesse de déplacement
	
	float rangeView;			//portée de la vue de l'entité
};

#endif
