/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 22/08/2015
 * 
 * Gestion de la carte du jeu
 ******************************************************************************/

#include <iostream>
#include "libtcod.hpp"
#include "constants.hpp"
#include "tools.hpp"
#include "engine.hpp"

#ifndef MAP_HPP
#define MAP_HPP

struct Tile
{
	Tile() : explored(false) {}

	bool explored;
};

class Map
{
public:
	Map(const int& pwidth, const int& pheight);
	~Map();

	void initialize();
	void generateByBSP();
	void render() const;
	void computeFov();

	bool isWall(const int &px, const int &py) const;
	bool isInFov(const int &px, const int &py) const;
	bool isExplored(const int &px, const int &py) const;
	bool canWalk(const int &px, const int &py) const;
	
	void addMonster(const int &px, const int &py);
	void addItem(const int &px, const int &py);

	void reveal();
	
	TCODRandom *rnd;

	int width;
	int height;
	
private:
	friend class BspListener;
	
	void dig(int px1, int py1, int px2, int py2);
	void createRoom(bool pfirst, int px1, int py1, int px2, int py2);

	Tile *tiles;
	TCODMap *map;

	long seed;
};

#endif