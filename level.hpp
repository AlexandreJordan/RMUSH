#ifndef Level_HPP
#define Level_HPP

#include "libtcod.hpp"
#include "constants.hpp"
#include "entitymonster.hpp"
#include "entityitem.hpp"
#include "entityfixeditem.hpp"
#include "tile.hpp"
#include <string>

class Room
{
public:
	Room(const int& px, const int& py, const int& pwidth, const int& pheight) : 
		x(px), y(py), width(pwidth), height(pheight) {}
	Room() : x(0), y(0), width(0), height(0) {}

	int x;
	int y;
	int width;
	int height;
};

class Level
{
public:
    Level(const int& pwidth, const int& pheight);
    ~Level();

	void generate();
	void generateBsp();
	void generateBspCorridor();
	void generateStairs();
	void generateFromFile(const std::string& ppath);
	
	void update();
	void render();

	void computeFov(const float& fovRadius);
	bool isWall(const int &px, const int &py) const;
	bool isInFov(const int &px, const int &py) const;
	bool canWalk(const int &px, const int &py) const;
	
	void reveal();
	void switchShowPnjsFov();
	
	TCODList<EntityMonster*>& getPnjsList() { return pnjs_; }
	TCODList<EntityItem*>& getItemsList() { return items_; }
	TCODList<EntityFixedItem*>& getFixedItemsList() { return fixedItems_; }
	
	int getWidth() const { return width_; }
	int getHeight() const { return height_; }
	
	Room* getFirstRoom() { return rooms_.get(0); }
	Room* getLastRoom() { return rooms_.peek(); }
	
	bool getShowPnjsFov() const { return showPnjsFov_; }
	
private:
	friend class BspListener;
	
	void dig(int px1, int py1, int px2, int py2);
	void createRoom(bool pfirst, int px1, int py1, int px2, int py2);
	
	bool isExplored(const int &px, const int &py) const;
	
	void addMonster(const int &px, const int &py);
	void addItem(const int& px, const int& py);
	
	void createStairUp(const int& px, const int& py);
	void createStairDown(const int& px, const int& py);
	EntityFixedItem* createGazTrap(const int& px, const int& py);
	void createLever(const int& px, const int& py, EntityFixedItem* pItemtoActive);
	void createRemoteControl(const int& px, const int& py, EntityItem* pItemtoActive);
	EntityItem* createMine(const int& px, const int& py);
	
	TCODRandom* rnd_;

	TCODMap* tcmap_;
	Tile* tiles_;
	
	TCODList<Room*> rooms_;
	TCODList<EntityMonster*> pnjs_;
	TCODList<EntityItem*> items_;
	TCODList<EntityFixedItem*> fixedItems_;
	
	bool showPnjsFov_;
	
	int width_;
	int height_;
};

#endif
