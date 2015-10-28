#ifndef Map_HPP
#define Map_HPP

#include "libtcod.hpp"
#include "constants.hpp"
#include "level.hpp"
#include <map>

class Map
{
public:
	Map();
	~Map();

	void initialize();
	void createLevel(const int& level, const int& pwidth, const int& pheight);
	
	void update();
	void render();
	
	void revealCurrentLevel();									//révèle le level courant
	
	Level& getCurrentLevel();									//accès au level courant
	
	void changeLevelToUp();										//change pour le level supérieur
	void changeLevelToDown();									//change pour le level inférieur
	
	int getCurrentLevelCount() const { return currentLevel_; }
private:
	int currentLevel_;											//gestion des levels
	std::map<int, Level*> levels_;								//...
};

#endif
