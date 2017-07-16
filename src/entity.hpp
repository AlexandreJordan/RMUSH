#ifndef Entity_HPP
#define Entity_HPP

#include "libtcod.hpp"
#include "pugixml.hpp"
#include <string>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                        ENTITY TYPE                                                          ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum EntityState
{
    IN_ACTION   = 0,
    WAIT        = 1
};

enum EntityItemType
{
    BANDAGE     = 0,
    MINE        = 1,
    WEAPON      = 2,
    ARMOR_HEAD  = 3,
    ARMOR_BODY  = 4,
    ARMOR_HANDS = 5,
    ARMOR_LEGS  = 6,
    ARMOR_FOOTS = 7,
    ALL         = 100
};

enum EntityPnjType
{
    HUMAN       = 0,
    DOG         = 1
};

enum EntityFixedItemType
{
    GAZTRAP     = 0,
    LEVER       = 1,
    PITFALL     = 2,
    STAIR_UP    = 3,
    STAIR_DOWN  = 4
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                             ///
///                                             ENTITY                                                          ///
///                                                                                                             ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Entity
{
public:
    Entity();
    virtual ~Entity() {}

    virtual void update()   = 0;
    virtual void rtupdate() = 0;
    virtual void render()   = 0;

    virtual void initLoad(const pugi::xml_node& pnode);
    virtual std::string getDataXml();

    void setPosition(const int& px, const int& py);             //modifie x et y

    int x;                                                      //position de l'entité
    int y;                                                      //...

    std::string name;                                           //nom de l'objet
    std::string description;                                    //description de l'objet

    bool block;                                                 //l'objet bloque le passage

    int chr;                                                    //représentation de l'entité
    TCODColor color;                                            //couleur de fond du caractère

    float timer;                                                //timer pour le rendu temps réel

    int speed;                                                  //vitesse d'exécution de l'entité
    int energy;                                                 //compteur permettant de débloquer l'action

    EntityState state;                                          //état de l'entité dans la gestion des actions

    int levelId;                                                //niveau actuel de l'entité dans la map

    int id;                                                     //id unique qui définit l'entité
};


/////////////////////////////////////////////////////////////////////////////////
///                                                                           ///
///                                  ENTITYPNJ                                ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////

class EntityPnj : public Entity
{
public:
    EntityPnj() : Entity(), life(0), maxLife(0),
        defense(0), strength(0), dexterity(0), stealth(0),
        isDead(false), moveSpeed(1), fov(10.0f), showFov(false) {}

    virtual ~EntityPnj() {}

    virtual void initNew(const int& px, const int& py, const int& pid,
                         const int& plevelId, const std::string&) = 0;          //initialisation lors d'une nouvelle partie
    virtual void initLoad(const pugi::xml_node& pnode) = 0;                     //initialisation lors d'un chargement d'une partie

    virtual void takeDamage(const float&) = 0;                                  //calcul et application des dommages sur l'entité

    //virtual bool moveOrAttack(const int& ptargetX, const int& ptargetY) = 0;    //action du pnj

    virtual bool action(const int& ptargetX, const int& ptargetY)   = 0;
    virtual bool move(const int& ptargetX, const int& ptargetY)     = 0;
    virtual bool attack(const int& ptargetX, const int& ptargetY)   = 0;

    virtual void update() = 0;

    std::string getDataXml();                                                   //retourne les données de l'entité en XML

    int life;                                                                   //niveau de vie actuel
    int maxLife;                                                                //vie maximale
    int defense;                                                                //point de défense
    int strength;                                                               //point d'attaque
    int dexterity;                                                              //point de dextérité
    int stealth;                                                                //furtivité du joueur
                                
    bool isDead;                                                                //l'entité est morte
    int moveSpeed;                                                              //vitesse de déplacement
                                
    float fov;                                                                  //portée de la vue de l'entité
    bool showFov;                                                               //affiche ou non le champ de vision

    EntityPnjType eType;
};


/////////////////////////////////////////////////////////////////////////////////
///                                                                           ///
///                                ENTITYITEM                                 ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////

class EntityItem : public Entity
{
public:
    EntityItem() : Entity(), isOnMap(true), isSelected(false) {}
    virtual ~EntityItem() {}

    virtual void initNew(const int&, const int&, const int&,
                         const int&, const std::string&)        = 0;        //initialisation lors d'une nouvelle partie
    virtual void initLoad(const pugi::xml_node& pnode)          = 0;        //initialisation lors d'un chargement d'une partie

    virtual void use(EntityPnj* powner)                      = 0;

    std::string getDataXml();

    bool isOnMap;
    bool isSelected;                                                        //sélecteur pour les listes
    EntityItemType eType;
};


/////////////////////////////////////////////////////////////////////////////////
///                                                                           ///
///                             ENTITYFIXEDITEM                               ///
///                                                                           ///
/////////////////////////////////////////////////////////////////////////////////

class EntityFixedItem : public Entity
{
public:
    EntityFixedItem() : Entity() {}
    virtual ~EntityFixedItem() {}

    virtual void initNew(const int&, const int&, const int&,
                         const int&, Entity* pitemLink = nullptr)   = 0;    //initialisation lors d'une nouvelle partie
    virtual void initLoad(const pugi::xml_node& pnode)              = 0;    //initialisation lors d'un chargement d'une partie
    virtual void use(EntityPnj* powner)                          = 0;

    std::string getDataXml();

    virtual void launch(EntityPnj* ptarget) {}

    EntityFixedItemType eType;
};

#endif
