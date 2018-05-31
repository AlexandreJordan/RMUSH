#ifndef GAZTRAP_HPP
#define GAZTRAP_HPP

#include "tools.hpp"
#include "pugixml.hpp"

#include <string>
#include <sstream>

class EntityFixedItem;

//
// GAZTRAP
//
// Dégage du gaz sur un rayon (radius) pendant une durée (duration) en tour
//
// Fonctionnement :
//  1 Déclenchement (launch()) :
//      isStarted = true;
//      on enregistre le tour au lancement du piège
//      on change l'aspect du piège (déjà utilisé)
//
//  2 Mise à jour tour par tour (update()) :
//      Mise à jour du compteur
//      Si la durée est terminée, on stop les effets du gaz
//
//  3 Mise à jour temps réel (rtupdate()) :
//      Mise à jour de la couleur des cases touchées par le gaz

class GazTrap : public EntityFixedItem
{
public:
    GazTrap();
    ~GazTrap();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink = nullptr);
    void initLoad(const pugi::xml_node& pnode);

    std::string getDataXml();

    void use(EntityPnj* powner);
    void launch(EntityPnj* powner);

    int radius;                 //diamètre du nuage

    int startRound;                 //enregistre le tour général lors du déclenchement
    //fin => startRound + duration
    bool isStarted;             //Vrai quand le piège est déclenché
};

#endif

#include "entity.hpp"
#include "engine.hpp"

GazTrap::GazTrap() :
    EntityFixedItem(), radius(5), startRound(0), isStarted(false)
{
}

GazTrap::~GazTrap()
{
}

void GazTrap::initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink)
{
    id          = pid;
    levelId     = plevelId;
    block       = false;
    chr         = '*';
    color       = C_ITEM_GAZTRAP;
    name        = "Gaztrap";
    description = "gaztrap";
    x           = px;
    y           = py;
    eType       = EntityFixedItemType::GAZTRAP;
}

void GazTrap::initLoad(const pugi::xml_node& pnode)
{
    EntityFixedItem::initLoad(pnode);

    radius      = pnode.child("radius").text().as_int();
    isStarted   = pnode.child("isStarted").text().as_bool();
}

std::string GazTrap::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityFixedItem::getDataXml();
    resXml << "<radius>" << radius << "</radius>" << std::endl;
    resXml << "<isStarted>" << isStarted << "</isStarted>" << std::endl;

    return resXml.str();
}


void GazTrap::use(EntityPnj* powner)
{
    Engine::getInstance()->getGui().message(TCODColor::green, "Un nuage de gaz s'échappe d'une trappe.");
}

//
// Update tour par tour
//
void GazTrap::update()
{
    if (!isStarted)
        return;

    isStarted = (startRound + T_DURATION_GAZTRAP >=  Engine::getInstance()->timeTotalRound);

    Level& currentLevel = Engine::getInstance()->getMap().getCurrentLevel();

    //modifier les propriétés de la case
    for (int nx = x - radius; nx <= x + radius; nx++)
        for (int ny = y - radius; ny <= y + radius; ny++)
        {
            if (!currentLevel.isInLevel(nx, ny))
                continue;

            if (currentLevel.isWall(x, y))
                continue;

            if (Tools::getDistance(nx, ny, x, y) > radius)
                continue;

            //ajoute un mutagène sur la case, il est calculé suivant sa position
            //par rapport au centre du piège et le nombre de tour déjà passé
            std::cout << "mod mutation affect"  << std::endl;
            currentLevel.getTile(nx, ny).mutationAffect = 0.1f;
        }
}

//
// Update temps réel
//
void GazTrap::rtupdate()
{
}

//
// Déclenchement automatique
//
void GazTrap::launch(EntityPnj* ptarget)
{
    Engine::getInstance()->getGui().message(TCODColor::green, "Vous marchez sur un gaztrap");

    isStarted   = true;
    startRound  = Engine::getInstance()->timeTotalRound;
    color   = TCODColor::darkAzure;
}

//
// Rendu
//
void GazTrap::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}

