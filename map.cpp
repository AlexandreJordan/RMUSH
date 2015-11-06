#include "map.hpp"
#include "constants.hpp"
#include "engine.hpp"
#include "tools.hpp"

/**
 * Constructeur
 */
Map::Map()
{
}

/**
 * Destructeur
 */
Map::~Map()
{
	levels_.clear();
}

/**
 * Initialisation de la carte
 * 
 * Génération d'un nombre aléatoire pour la génération de la carte basé sur un seed
 * création de la carte (tile)
 * création de la carte (fov)
 * création d'un bsp tree pour la génération aléatoire
 * 
 * Accès à une case x,y -> map[x + y * width]
 */
void Map::initialize()
{
	currentLevel_ = 0;
	
	//DEV création des levels du jeu
	createLevel(0, MAP_WIDTH, MAP_HEIGHT);
	createLevel(1, MAP_WIDTH, MAP_HEIGHT);
	createLevel(2, MAP_WIDTH, MAP_HEIGHT);
	createLevel(3, MAP_WIDTH, MAP_HEIGHT);
}

/**
 * Création d'un level
 */
void Map::createLevel(const int& plevel, const int& pwidth, const int& pheight)
{
	Level* lvl = new Level(pwidth, pheight);
	lvl->generate();
	lvl->generateBsp();
	lvl->generateBspCorridor();
	levels_[plevel] = lvl;
}
 
/**
 * Dessin du level courant
 */
void Map::render()
{
	levels_[currentLevel_]->render();
}

/**
 * Mise à jour du level courant
 */
void Map::update()
{
	levels_[currentLevel_]->update();
}

/**
 * Retourne le level courant
 */
Level& Map::getCurrentLevel()
{
	return *levels_[currentLevel_];
}

/**
 * Révèle le level courant
 */
void Map::revealCurrentLevel()
{
	levels_[currentLevel_]->reveal();
}

/**
 * Change pour le level supérieur
 */
void Map::changeLevelToUp()
{
	//le level courant est le premier
	//-> on sort
	if (currentLevel_ <= 0)
		return;
	
	currentLevel_--;
	
	//position du joueur dans la pièce de l'escalier opposé
	Engine::getInstance()->getPlayer().setPosition(	levels_[currentLevel_]->getLastRoom()->x,
													levels_[currentLevel_]->getLastRoom()->y );
}

/**
 * Change pour le level inférieur
 */
void Map::changeLevelToDown()
{
	//le level courant est le dernier
	//-> on sort
	if ((unsigned)currentLevel_ >= levels_.size())
		return;
	
	currentLevel_++;
	
	//position du joueur dans la pièce de l'escalier opposé
	Engine::getInstance()->getPlayer().setPosition(	levels_[currentLevel_]->getFirstRoom()->x,
													levels_[currentLevel_]->getFirstRoom()->y );
}
