#include "map.hpp"

#include <sstream>
#include <vector>

#include "engine.hpp"
#include "constants.hpp"
#include "tools.hpp"
#include "level.hpp"

using namespace std;

Map::~Map()
{
    for (std::map<int, Level*>::iterator it = levels_.begin(); it != levels_.end(); it++)
        delete (*it).second;

    levels_.clear();
}

//
// Initialisation de la carte pour une nouvelle partie
//
// Premier level : 0
// Création des niveaux aléatoirement
//
void Map::initNew()
{
    currentLevel_ = 0;

    for (int i = 0; i < LEVELS_COUNT; i++)
        createLevel(i, MAP_WIDTH, MAP_HEIGHT);
}

//
// Initialisation de la carte avec des données
//
//      pnode == <levels>
//
void Map::initLoad(const int& pcurrentLevel, const pugi::xml_node& pnode)
{
    currentLevel_ = pcurrentLevel;

    for (pugi::xml_node levelNode: pnode.children("level"))
    {
        int levelId = levelNode.attribute("id").as_int();

        Level* lvl = new Level(MAP_WIDTH, MAP_HEIGHT, levelId);
        lvl->generate();
        lvl->initLoad(levelNode);
        levels_[levelId] = lvl;
    }
}


//
// Création d'un level de façon aléatoire
//
// Génération d'un nombre aléatoire pour la génération de la carte basé sur un seed
// création de la carte (tile)
// création de la carte (fov)
// création d'un bsp tree pour la génération aléatoire
//
void Map::createLevel(const int& plevel, const int& pwidth, const int& pheight)
{
    Level* lvl = new Level(pwidth, pheight, plevel);
    lvl->generate();
    lvl->generateBsp();
    lvl->generateBspCorridor();
    lvl->generateStairs();
    lvl->populate();
    levels_[plevel] = lvl;
}

//
// Dessin du level courant
//
void Map::render()
{
    levels_[currentLevel_]->render();
}

//
// Mise à jour du level courant
//
void Map::update()
{
    levels_[currentLevel_]->update();
}

//
// Retourne le level courant
//
Level& Map::getCurrentLevel()
{
    return *levels_[currentLevel_];
}

//
// Révèle le level courant
//
void Map::revealCurrentLevel()
{
    levels_[currentLevel_]->reveal();
}

//
// Change pour le level supérieur
//
void Map::changeLevelToUp()
{
    //le level courant est le premier
    if (currentLevel_ <= 0)
        return;

    currentLevel_--;

    Engine* engine = Engine::getInstance();

    EntityFixedItem* stair = engine->getEntityManager().getFixedItem(levels_[currentLevel_]->getStairDownId());
    engine->getPlayer().setPosition(stair->x, stair->y);
}

//
// Change pour le level inférieur
//
void Map::changeLevelToDown()
{
    //le level courant est le dernier
    if (currentLevel_ >= levels_.size())
        return;

    currentLevel_++;

    Engine* engine = Engine::getInstance();

    EntityFixedItem* stair = engine->getEntityManager().getFixedItem(levels_[currentLevel_]->getStairUpId());
    engine->getPlayer().setPosition(stair->x, stair->y);
}

//
// Retourne les données sous forme de XML
//
std::string Map::getDataXml()
{
    stringstream resXml;

    resXml << "<currentLevel>" << currentLevel_ << "</currentLevel>" << std::endl;

    resXml << "<levels>" << endl;

    for (auto& key: levels_)
        resXml << key.second->getDataXml() << std::endl;

    resXml << "</levels>" << endl;

    return resXml.str();
}

