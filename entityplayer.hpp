#ifndef EntityPlayer_HPP
#define EntityPlayer_HPP

#include "entitymobile.hpp"

class EntityPlayer : public EntityMobile
{
public:
	EntityPlayer();
    EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense);
    ~EntityPlayer();

	void update();
	void render();
	
private:
	void processKey(const int& key);
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);
};

#endif
