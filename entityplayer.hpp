#ifndef EntityPlayer_HPP
#define EntityPlayer_HPP

#include "entitymobile.hpp"
#include "entityitem.hpp"
#include "entityfixeditem.hpp"

class EntityPlayer : public EntityMobile
{
public:
	EntityPlayer();
    EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense);
    ~EntityPlayer();

	void update();
	void render();
	
	TCODList<EntityItem*>& getInventory() { return inventory_; }
	
private:
	void processKey(const int& key);											//gestion de la touche appuyées
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);				//déplacement ou attaque du joueur
	
	EntityItem* choseOneFromList(TCODList<EntityItem*> plist);					//retourne un item choisi dans plist
	TCODList<EntityItem*> choseMultiFromList(TCODList<EntityItem*> plist);		//retourne une liste d'item choisi dans plist
	
	bool addToInventory(EntityItem* item);										//ajoute l'item dans l'inventaire
	bool dropToGround(EntityItem* item);										//dépose un objet au sol
	
	EntityFixedItem* getFixedItemFromTile(const int& px, const int& py);		//retourne un pointeur sur l'item au sol
	TCODList<EntityItem*> getItemsFromTile(const int& px, const int& py);		//retourne une liste d'item sur une case de la carte
	
	int inventorySize_;
	TCODList<EntityItem*> inventory_;
};

#endif
