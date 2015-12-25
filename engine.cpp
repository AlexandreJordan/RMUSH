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
	TCODConsole::setCustomFont("./data/fonts/font-5.png", (TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW));
	TCODConsole::initRoot(WINDOW_WIDTH, WINDOW_HEIGHT, "RMush - AJ", false);
	TCODSystem::setFps(60);
	TCODConsole::root->mapAsciiCodesToFont(0, 255, 0, 0);
	TCODConsole::root->setKeyboardRepeat(175, 30);
	
	//création du moteur de nombre aléatoire et génération
	//d'une seed pour pouvoir regénérer les mêmes éléments
	mainSeed 		= TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
	randomEngine	= new TCODRandom(mainSeed, TCOD_RNG_CMWC);
	
	
	//initialisation de la carte
	map_.initialize();
	
	//DEV initializePlayer
	initializePlayer();
	
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
		map_.getCurrentLevel().computeFov(player_.fov);
	
	mainStatus_ = IDLE;

	//touche appuyée
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &mainKey_, NULL);
	
	//gestion des touches systèmes
	switch(mainKey_.vk)
	{
		//affiche toute la carte
        case TCODK_INSERT	: { map_.revealCurrentLevel(); isRevealMode_ = true; break; }
		//vide la liste des messages
		case TCODK_DELETE	: gui_.clear(); break;
		//affiche la console de développement
		case TCODK_F2		: showDevConsole(); break;
		//affiche l'aide
		case TCODK_F1		: showHelp(); break;
        default: break;
	}
	
	//mise à jour du joueur
	player_.update();
	player_.rtupdate();
	
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
	
	//dessin du gui
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
	player_.fov			= 5;
	player_.x			= Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->x;
	player_.y			= Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->y;
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
	
	TCOD_key_t key;
	bool isClosed = false;

	//
	//boucle d'affichage
	//
	while (!isClosed)
	{
		//système
		console.print(1, 2, "-> Systeme");
		console.print(1, 4, "FPS : %i", TCODSystem::getFps());
		//map
		console.print(1, 10, "-> Carte");
		console.print(1, 12, "Taille : %i, %i", map_.getCurrentLevel().getWidth(), map_.getCurrentLevel().getHeight());
		console.print(1, 13, "Joueur position : %i, %i", player_.x, player_.y);
		console.print(1, 14, "Level courant : %i", map_.getCurrentLevelCount());
		
		//items/objets
		console.print(28, 2, "-> Mobile");
		console.print(28, 4, "Monstres : %i", map_.getCurrentLevel().getPnjsList().size());
		console.print(28, 5, "Items : %i", map_.getCurrentLevel().getItemsList().size());
		
		//vars
		console.print(1, 20, "-> Vars");
		console.print(1, 21, "FOV pnjs : %i", map_.getCurrentLevel().getShowPnjsFov());

		//gestion de la console
		TCODConsole::blit(&console, 0, 0, 
					  DEV_CONSOLE_WIDTH, 
					  DEV_CONSOLE_HEIGHT, 
					  TCODConsole::root, 
					  (int)(WINDOW_WIDTH - DEV_CONSOLE_WIDTH) / 2, 
					  (int)(WINDOW_HEIGHT - DEV_CONSOLE_HEIGHT) / 2);
		TCODConsole::flush();
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

		switch(key.vk)
		{
			case TCODK_ESCAPE 	: isClosed  = true; break;
			case TCODK_F1		: map_.getCurrentLevel().switchShowPnjsFov(); break;
			default				: break;
		}
	}
}

/**
 * Affiche l'aide du jeu
 *	- Liste des commandes
 */
void Engine::showHelp()
{
	TCODConsole console(HELP_CONSOLE_WIDTH, HELP_CONSOLE_HEIGHT);
	console.setDefaultBackground(TCODColor(200, 180, 50));
	console.printFrame(0, 0, HELP_CONSOLE_WIDTH, HELP_CONSOLE_HEIGHT, true, TCOD_BKGND_DEFAULT, "Aide");
	console.setDefaultForeground(TCODColor::white);

	TCOD_key_t key;

	//système
	console.print(1, 3, "Commandes");
	console.print(1, 5, "Déplacement");
	console.print(1, 6, "    8");
	console.print(1, 7, "    |");
	console.print(1, 8, "4 ----- 6");
	console.print(1, 9, "    |");
	console.print(1, 10, "    2");
	console.print(1, 12, "Monter / Descendre                : < >");
	console.print(1, 15, "Récupérer un item                 : g");
	console.print(1, 16, "Déposer un item                   : d");
	console.print(1, 17, "Utiliser un objet de l'inventaire : a");
	console.print(1, 18, "Utiliser un objet au sol          : u");
	console.print(1, 19, "Ouvrir l'inventaire               : i");
	console.print(1, 20, "Afficher les items d'une case     : x");

	//gestion de la console
	TCODConsole::blit(&console, 0, 0,
		HELP_CONSOLE_WIDTH,
		HELP_CONSOLE_HEIGHT,
		TCODConsole::root,
		(int)(WINDOW_WIDTH - HELP_CONSOLE_WIDTH) / 2,
		(int)(WINDOW_HEIGHT - HELP_CONSOLE_HEIGHT) / 2);
	TCODConsole::flush();
	TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

	if (key.vk == TCODK_ESCAPE)
		return;
}
