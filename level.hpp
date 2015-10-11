#ifndef Level_HPP
#define Level_HPP

#include "libtcod.hpp"
#include "constants.hpp"
#include "entitymonster.hpp"
#include "entityitem.hpp"
#include "tile.hpp"

class Level
{
public:
    Level(const int& pwidth, const int& pheight);
    ~Level();

	void generate();
	void generateBsp();
	
	void update();
	void render();

	void computeFov();
	bool isWall(const int &px, const int &py) const;
	bool isInFov(const int &px, const int &py) const;
	bool canWalk(const int &px, const int &py) const;
	
	void reveal();
	
	TCODList<EntityMonster*>& getMonstersList() { return monsters_; }
	TCODList<EntityItem*>& getItemsList() { return items_; }

private:
	friend class BspListener;
	
	void dig(int px1, int py1, int px2, int py2);
	void createRoom(bool pfirst, int px1, int py1, int px2, int py2);
	bool isExplored(const int &px, const int &py) const;
	void addMonster(const int &px, const int &py);
	void addItem(const int& px, const int& py);
	
	TCODRandom* rnd_;

	TCODMap* tcmap_;
	Tile* tiles_;
	
	TCODList<EntityMonster*> monsters_;
	TCODList<EntityItem*> items_;
	
	int width_;
	int height_;
};

#endif
