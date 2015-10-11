#include "engine.hpp"
#include "constants.hpp"

Engine *Engine::instance_ = 0;

/**
 * Initialisation du moteur de jeu
 */
void Engine::initialize()
{
	//création de la console principale
	//définition de la limite des FPS
	TCODConsole::initRoot(WINDOW_WIDTH, WINDOW_HEIGHT, "RMush - AJ", false);
	TCODSystem::setFps(60);
	
	//création du moteur de nombre aléatoire et génération
	//d'une seed pour pouvoir regénérer les mêmes éléments
	mainSeed 		= TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
	randomEngine	= new TCODRandom(mainSeed, TCOD_RNG_CMWC);
	
	//DEV initializePlayer
	initializePlayer();
	
	//initialisation de la carte
	map_.initialize();
	
	//DEV
	mainStatus_ = STARTUP;
}

/**
 * Mise à jour du jeu
 */
void Engine::update()
{
	//départ du jeu, on génère le FOV du joueur
	if (mainStatus_ == STARTUP)
		map_.getCurrentLevel().computeFov();
	
	mainStatus_ = IDLE;

	//touche appuyée
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &mainKey_, NULL);
	
	//gestion des touches systèmes
	switch(mainKey_.vk)
	{
		//affiche toute la carte
        case TCODK_INSERT	: { map_.revealCurrentLevel(); isRevealMode_ = true; break; }
		//vide la liste des messages
		//case TCODK_DELETE	: gui->clear(); break;
		//affiche la console de développement
		case TCODK_F2		: showDevConsole(); break;
        default: break;
	}
	
	//mise à jour du joueur
	player_.update();
	
	//mise à jour de la carte
	map_.update();
}

/**
 * Dessin du jeu
 */
void Engine::render()
{
	TCODConsole::root->clear();
	
	//dessin de la carte
	map_.render();
	
	//dessin du joueur
	player_.render();
	
	//TODO dessin du gui
	gui_.render();
}

/**
 * Initialisation du joueur
 */
void Engine::initializePlayer()
{
	//création du joueur
	player_.block 		= true;
	player_.chr			= '@';
	player_.color		= C_PLAYER;
	player_.description	= "Vous...";
	player_.name		= "kz";
	player_.life		= 40.0f;
	player_.maxLife		= 40.0f;
	player_.defense		= 0;
	player_.x			= 0;
	player_.y			= 0;
}

/**
 * Affiche la console de développement - Informations sur le jeu
 */
void Engine::showDevConsole()
{
	TCODConsole console(DEV_CONSOLE_WIDTH, DEV_CONSOLE_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, DEV_CONSOLE_WIDTH, DEV_CONSOLE_HEIGHT, true, TCOD_BKGND_DEFAULT, "devconsole");
	console.setDefaultForeground(TCODColor::white);
	
	//système
	console.print(1, 1, "Systeme");
	console.print(1, 2, "FPS : %i", TCODSystem::getFps());
	//map
	//console.print(1, 4, "Carte");
	//console.print(1, 5, "Taille : %i, %i", map_.width, map->height);
	console.print(1, 7, "Joueur position : %i, %i", player_.x, player_.y);
	//items/objets
	//console.print(25, 1, "Monstres : %i", getMonstersCount());
	//console.print(25, 2, "Items : %i", getItemsCount());
	
	TCODConsole::blit(&console, 0, 0, 
						DEV_CONSOLE_WIDTH, 
						DEV_CONSOLE_HEIGHT, 
						TCODConsole::root, 
						(int)(WINDOW_WIDTH - DEV_CONSOLE_WIDTH) / 2,
						(int)(WINDOW_HEIGHT - DEV_CONSOLE_HEIGHT) / 2);
	TCODConsole::flush();
	
	TCOD_key_t key;
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
	
	if (key.vk == TCODK_CHAR)
		return;
}
