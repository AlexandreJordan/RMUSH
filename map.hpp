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
	
	void revealCurrentLevel();					//révèle le level courant
	
	Level& getCurrentLevel();					//accès au level courant
	
private:
	int currentLevel_;							//gestion des levels
	std::map<int, Level*> levels_;				//...
};

#endif
