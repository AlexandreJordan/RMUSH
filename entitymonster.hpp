#ifndef EntityMonster_HPP
#define EntityMonster_HPP

#include "entitymobile.hpp"

class EntityMonster : public EntityMobile
{
public:
	EntityMonster();
    EntityMonster(const float& plife, const float& pMaxLife, const float& pdefense);
    virtual ~EntityMonster();

	void update();
	void render();

private:
	void moveOrAttack(const int& ptargetX, const int& ptargetY);
};

#endif
