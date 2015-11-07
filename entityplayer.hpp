#ifndef EntityPlayer_HPP
#define EntityPlayer_HPP

#include "entitymobile.hpp"
#include "entityitem.hpp"
#include "entityfixeditem.hpp"
#include <string>

class EntityPlayer : public EntityMobile
{
public:
	EntityPlayer();
    EntityPlayer(const float& plife, const float& pMaxLife, const float& pdefense, const float& pfov);
    ~EntityPlayer();

	void update();
	void render();
	
	TCODList<EntityItem*>& getInventory() { return inventory_; }
	
private:
	void processKey(const int& key);														//gestion de la touche appuyées
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);							//déplacement ou attaque du joueur
	
	EntityItem* choseOneFromList(TCODList<EntityItem*> plist, const std::string& title);	//retourne un item choisi dans plist
	TCODList<EntityItem*> choseMultiFromList(TCODList<EntityItem*> plist);					//retourne une liste d'item choisi dans plist
	
	bool addToInventory(EntityItem* item);													//ajoute l'item dans l'inventaire
	void dropToGround(EntityItem* item);													//dépose un objet au sol
	void selectTile(int&px, int& py, const float& prange);
	
	EntityFixedItem* getFixedItemFromTile(const int& px, const int& py);					//retourne un pointeur sur l'item au sol
	TCODList<EntityItem*> getItemsFromTile(const int& px, const int& py);					//retourne une liste d'item sur une case de la carte
	
	int inventorySize_;
	TCODList<EntityItem*> inventory_;
};

#endif
