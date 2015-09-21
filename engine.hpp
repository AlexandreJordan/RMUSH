#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "main.hpp"

enum GameStatus {
    STARTUP = 0,
    IDLE = 1,
    NEW_TURN = 2,
    WIN = 3,
    LOSE = 4
};

class Engine
{
public:
    Engine();
    ~Engine();

    void update();
    void render();
	
	void initialize();
	void load();
	void save();

    Object* getPlayer() { return player; }
	Object* getStair() { return stair; }
    Map* getMap() { return map; }
    Gui* getGui() { return gui; }
    TCODList<Object*>& getObjects() { return objects; }
    int getFovRadius() const { return fovRadius; }
	
	GameStatus getGameStatus() const { return gameStatus; }
	void setGameStatus(const GameStatus& pgameStatus) { gameStatus = pgameStatus; }

	void insertObjectStartList(Object* obj);
	Object* getClosestObject(const int& px, const int& py, const float& range);
	bool pickTile(int& px, int& py, const float& range);
	void inspectTile();
	
	void showDevConsole();
	
	int getMonstersCount() const;
	int getItemsCount() const;
	
	TCOD_key_t key;

    static Engine* getInstance()
    {
	if (!instance)
	    instance = new Engine();

	return instance;
    }

private:
    TCODList<Object*> objects;
    Object* player;
	Object* stair;
    Map* map;
    Gui* gui;
    int fovRadius;
    bool computeFov;
	bool isRevealMode;
    GameStatus gameStatus;

    static Engine* instance;
};

#endif