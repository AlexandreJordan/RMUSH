#ifndef MAP_HPP
#define MAP_HPP

#include <map>
#include <iostream>
#include <string>
#include "pugixml.hpp"

class Level;

class Map
{
public:
    Map() {}
    ~Map();

    void initNew();                                 //initialisation de la map pour une nouvelle partie
    void initLoad(const int& pcurrentLevel, const pugi::xml_node& pnode);           //initialisation de la map avec données

    void createLevel(const int& level, const int& pwidth, const int& pheight);

    void update();
    void render();

    void revealCurrentLevel();                              //révèle le level courant

    Level& getCurrentLevel();                               //accès au level courant
    std::map<int, Level*>& getLevels() {
        return levels_;    //accès à la liste des levels
    }

    void changeLevelToUp();                             //change pour le level supérieur
    void changeLevelToDown();                               //change pour le level inférieur

    int getCurrentLevelId() const {
        return currentLevel_;
    }

    std::string getDataXml();

private:
    unsigned int currentLevel_;                             //gestion des levels
    std::map<int, Level*> levels_;                          //...
};

#endif
