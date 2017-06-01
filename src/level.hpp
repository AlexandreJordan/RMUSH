#ifndef Level_HPP
#define Level_HPP

#include "entity.hpp"
#include "entitypnj.hpp"
#include "pugixml.hpp"

#include <list>
#include <string>

//
// Représentation des pièces du niveau
//
class Room
{
public:
    Room(const int& px, const int& py, const int& pwidth, const int& pheight) :
        x(px), y(py), width(pwidth), height(pheight) {}
    Room() : x(0), y(0), width(0), height(0) {}

    int x;
    int y;
    int width;
    int height;
};

//
// Case du niveau avec ses propriétés
//
struct Tile
{
    Tile() : explored(false), isDig(false), isFire(false), isFall(false), mutationAffect(0.0f) {}

    bool explored;      //VRAI si la case est déjà exploré
    bool isDig;         //VRAI si la case est creusé et donc accessible
    bool isFire;        //VRAI si la case est actuellement en feu
    bool isFall;        //VRAI si la case est un trou (donnant accès à l'étage inférieur)
    float mutationAffect;   //indique le "taux" de mutagène présent sur la case
};


//
// Niveau complet
// Contient les Pnjs, items, items fixes
//
class Level
{
public:
    Level(const int& pwidth, const int& pheight, const int& pid);
    ~Level();

    void update();
    void render();

    void initLoad(const pugi::xml_node& pnode);

    void generate();                                        //fonctions utilisées lors d'une
    void generateBsp();                                     //nouvelle partie
    void generateBspCorridor();                             // --
    void generateStairs();                                  // --
    void populate();                                        // --

    void computeFov(const float& fovRadius);
    bool isWall(const int& px, const int& py) const;
    bool isInFov(const int& px, const int& py) const;
    bool canWalk(const int& px, const int& py) const;
    bool isInLevel(const int& px, const int& py) const;

    void digRectangle(int px1, int py1, int px2, int py2);
    void digCircle(const int& px, const int& py, const int& radius);
    void digCircleFall(const unsigned int& px, const unsigned int& py, const unsigned int& radius);

    void deleteItemsInCircle(const unsigned int& px, const unsigned int& py, const float& radius, EntityItem* pexclude);
    void deleteFixedItemsInCircle(const unsigned int& px, const unsigned int& py, const float& radius, EntityFixedItem* pexclude);

    void reveal();
    void switchShowPnjsFov();

    Tile& getTile(const unsigned int& px, const unsigned int& py) {
        return tiles_[px + py * width_];
    }

    TCODList<Room*>& getRooms() {
        return rooms_;
    }

    int getWidth() const {
        return width_;
    }
    int getHeight() const {
        return height_;
    }

    Room* getFirstRoom() {
        return rooms_.get(0);
    }
    Room* getLastRoom() {
        return rooms_.peek();
    }

    bool getShowPnjsFov() const {
        return showPnjsFov_;
    }

    int getId() {
        return id_;
    }

    int getStairUpId() {
        return stairUpId_;
    }

    int getStairDownId() {
        return stairDownId_;
    }

    std::string getDataXml();

private:
    friend class BspListener;


    void createRoom(int px1, int py1, int px2, int py2);

    bool isExplored(const int& px, const int& py) const;

    TCODRandom* rnd_;

    TCODMap* tcmap_;
    Tile* tiles_;

    TCODList<Room*> rooms_;

    int stairUpId_;
    int stairDownId_;

    bool showPnjsFov_;

    int width_;
    int height_;

    unsigned int id_;                               //ID du level
};

#endif
