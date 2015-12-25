#ifndef Engine_HPP
#define Engine_HPP

#include <iostream>
#include "map.hpp"
#include "gui.hpp"
#include "entityplayer.hpp"
#include "entityitem.hpp"
#include "entitymonster.hpp"

enum GameStatus
{
    STARTUP 	= 0,
    IDLE 		= 1,
    NEW_TURN 	= 2,
    WIN 		= 3,
    LOSE 		= 4
};

class Engine
{
public:
    Engine() {}
    ~Engine() {}

	void render();
	void update();
	
	void initialize();
	
	TCODRandom* randomEngine;							//classe de random initialisé sur mainSeed
	long mainSeed;										//...
	
	EntityPlayer& getPlayer() { return player_; }		//accès public au joueur
	TCOD_key_t& getMainKey() { return mainKey_; }		//accès à la touche appuyée
	Map& getMap() { return map_;}						//accès à la carte
	Gui& getGui() { return gui_; }						//accès au Gui
	bool getRevealMode() { return isRevealMode_; }		//accès au mode reveal
	GameStatus& getMainStatus() { return mainStatus_; }	//accès au statut en cours du jeu
	void setMainStatus(const GameStatus& status) {		//modification du status du jeu
		mainStatus_ = status;
	}
	
	static Engine* getInstance() {
		if (!instance_)
			instance_ = new Engine();

		return instance_;
	}
	
private:
	void initializePlayer();							//création et initialisation du joueur
	void showDevConsole();								//affiche la console de développement
	void showHelp();									//affiche la console d'aide

	EntityPlayer player_;								//le joueur
	
	Map map_;											//gestion de la carte
	Gui gui_;											//gestion du gui
	
	TCOD_key_t mainKey_;								//touche appuyé durant l'update
	
	GameStatus mainStatus_;								//statut actuel du jeu
	
	bool isRevealMode_;									//affiche l'ensemble de la carte et des PNJs/items
	
	static Engine* instance_;							//instance unique du moteur
};

#endif
