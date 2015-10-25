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
	
	void useItemFromGround(EntityItem* item);
	
	TCODList<EntityItem*>& getInventory() { return inventory_; }
	
private:
	void processKey(const int& key);
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);
	
	EntityItem* choseOneFromList(TCODList<EntityItem*> plist);
	TCODList<EntityItem*> choseMultiFromList(TCODList<EntityItem*> plist);
	
	bool addToInventory(EntityItem* item);
	bool dropToGround(EntityItem* item);
	
	TCODList<EntityItem*> getItemsFromTile(const int& px, const int& py);
	
	
	int inventorySize_;
	TCODList<EntityItem*> inventory_;
};

#endif
