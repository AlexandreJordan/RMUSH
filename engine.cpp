#include "engine.hpp"

Engine *Engine::instance = NULL;

/**
 * Constructeur
 * 	- Initialisation du rayon du FOV
 * 	- Création de la fenêtre principale
 * 	- Création du GUI
 */
Engine::Engine() : fovRadius(10), gameStatus(STARTUP)
{
	TCODConsole::initRoot(WINDOW_WIDTH, WINDOW_HEIGHT, "RMush - AJ", false);
	TCODSystem::setFps(60);

	gui	= new Gui();
	isRevealMode = false;
}

/**
 * Destructeur 
 */
Engine::~Engine()
{	
	objects.clearAndDelete();
	delete map;
	delete gui;
}

/**
 * Initialisation du moteur
 * 	- Création du joueur
 * 	- Création de la carte
 * 	- //TODO Peupler de monstres et items pour dev
 */
void Engine::initialize()
{
	//création du joueur
	player	= new Object(10, 10, '@', "joueur1", "Vous... gros nul", true, TCODColor::white);
	player->destructible 	= new PlayerDestructible(player, 50.0f, 50.0f);
	player->attacker		= new Attacker(player, 5);
	player->ai				= new PlayerAi(player);
	player->container		= new Container(20);
	objects.push(player);
	
	//DEV création de l'escalier
	stair = new Object(0, 0, '>', "Escalier", "Descendre...", false, C_CHAR_STAIR);
	objects.push(stair);
	
	//DEV arme de base pour le joueur
	Object* cross = new Object(0, 0, 'D', "arba", "super arba", false, TCODColor::yellow);
	cross->usable = new CrossBow(cross, 10, 50);
	player->container->add(cross);
	
	map	= new Map(MAP_WIDTH, MAP_HEIGHT);
	map->initialize();
	map->generateByBSP();
}

void Engine::save()
{
}

void Engine::load()
{
	initialize();
}

/**
 * Mise à jour de la scène
 */
void Engine::update()
{
	//départ du jeu, on génère le FOV du joueur
	if (gameStatus == STARTUP)
		map->computeFov();
	
	gameStatus = IDLE;

	//touche appuyée
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
	
	//gestion des touches systèmes
	switch(key.vk)
	{
		//affiche toute la carte
        case TCODK_INSERT	: { map->reveal(); isRevealMode = true; break; }
		//vide la liste des messages
		case TCODK_DELETE	: gui->clear(); break;
		//affiche la console de développement
		case TCODK_F2		: showDevConsole(); break;
        default: break;
	}
	
	//mise à jour du joueur
	player->update();
	
	//mise à jour de la carte
	//personnages, objets, etc...
	if (gameStatus == NEW_TURN)
	{
		for (Object **iterator = objects.begin(); iterator != objects.end(); iterator++)
		{
			if ((*iterator) != player)
				(*iterator)->update();
		}
	}
}

/**
 * Rendu de la scène
 * 	- Rendu de la carte
 * 	- Rendu des objets
 * 	- Rendu du GUI
 */
void Engine::render()
{
	TCODConsole::root->clear();
	
	map->render();
	
	for (Object **iterator = objects.begin(); iterator != objects.end(); iterator++)
	{
		if (map->isInFov((*iterator)->posX, (*iterator)->posY) || isRevealMode)
			(*iterator)->render();
	}
	
	gui->render();
}

/**
 * Déplace l'objet en début de liste des "Objects"
 */
void Engine::insertObjectStartList(Object* obj)
{
	objects.remove(obj);
	objects.insertBefore(obj, 0);
}

/**
 * Récupère l'objet le plus proche du point donnée suivant le range
 * @param px Position de base
 * @param py Position de base
 * @param range Rayon à vérifier
 */
Object* Engine::getClosestObject(const int& px, const int& py, const float& range)
{
	Object* closest 		= NULL;
	float distance 			= 0.0f;
	float closestDistance 	= 1E6F;
	
	for (Object** it = objects.begin(); it != objects.end(); it++)
	{
		Object* obj = *it;
		
		if (obj != player && obj->destructible && !obj->destructible->isDead())
		{
			distance = obj->getDistance(px, py);
			//std::cout << "distance : " << distance << std::endl;
			
			if (distance < closestDistance) {
				closestDistance = distance;
				closest 		= obj;
			}
		}
	}
	
	return closest;
}


bool Engine::pickTile(int& px, int& py, const float& range = 0.0f)
{
	int lastX = player->posX;
	int lastY = player->posY;
	
	//nouvelle boucle principale
	while (!TCODConsole::isWindowClosed())
	{
		render(); //rendu principal
		
		//mettre en avant les case dans le FOV et est dans le range
		for (int mx = 0; mx < map->width; mx++)
			for (int my = 0; my < map->height; my++)
			{
				if (map->isInFov(mx, my) && (range == 0 || player->getDistance(mx, my) <= range))
				{
					TCODColor col = TCODConsole::root->getCharBackground(mx, my);
					col = col * 1.2f;
					TCODConsole::root->setCharBackground(mx, my, col);
				}
			}
		
		TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);
		
		switch (key.vk)
		{
			case TCODK_LEFT : {
				lastX -= 1;
				break;
			}
			case TCODK_RIGHT : {
				lastX += 1;
				break;
			}
			case TCODK_UP : {
				lastY -= 1;
				break;
			}
			case TCODK_DOWN : {
				lastY += 1;
				break;
			}
			case TCODK_ENTER : {
				px = lastX;
				py = lastY;
				return true;
			}
				
			case TCODK_ESCAPE :
				return false;
				
			default :
				return false;
		}

		//position du curseur
		TCODConsole::root->setCharBackground(lastX, lastY, TCODColor::white);
		
		TCODConsole::flush();
	}
	
	return false;
}

/**
 * Retourne le nombre de monstres générés sur la carte
 */
int Engine::getMonstersCount() const
{
	int count = 0;
	for (Object** it = objects.begin(); it != objects.end(); it++) {
		Object* obj = *it;
		if (obj->attacker && obj != player)
			count++;
	}
	return count;
}

/**
 * Retourne le nombre d'items générés sur la carte
 */
int Engine::getItemsCount() const
{
	int count = 0;
	for (Object** it = objects.begin(); it != objects.end(); it++) {
		Object* obj = *it;
		if (obj->usable && obj != player)
			count++;
	}
	return count;
}

/**
 * TODO Affiche dans une console la liste des objets présent sur la case
 */
void Engine::inspectTile()
{
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
	console.print(1, 4, "Carte");
	console.print(1, 5, "Taille : %i, %i", map->width, map->height);
	console.print(1, 7, "Joueur position : %i, %i", player->posX, player->posY);
	
	//items/objets
	console.print(25, 1, "Monstres : %i", getMonstersCount());
	console.print(25, 2, "Items : %i", getItemsCount());
	
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
