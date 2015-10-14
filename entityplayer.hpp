#ifndef EntityPlayer_HPP
#define EntityPlayer_HPP

#include "entitymobile.hpp"
#include "entityitem.hpp"

class EntityPlayer : public EntityMobile
{
public:
	EntityPlayer();
    EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense);
    ~EntityPlayer();

	void update();
	void render();
	
	bool addToInventory(EntityItem* item);
	void useItem(EntityItem* item);
	
	TCODList<EntityItem*>& getInventory() { return inventory_; }
	
private:
	void processKey(const int& key);
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);
	EntityItem* choseFromInventary();
	
	int inventorySize_;
	TCODList<EntityItem*> inventory_;
};

#endif
