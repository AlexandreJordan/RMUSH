#ifndef Engine_HPP
#define Engine_HPP

#include <list>
#include <vector>
#include <string>

#include "map.hpp"
#include "gui.hpp"
#include "entityplayer.hpp"
#include "entitymanager.hpp"
#include "datamanager.hpp"
#include "animationmanager.hpp"

enum class GameStart
{
    NEW_GAME,
    LOAD_GAME
};

enum class GameStatus
{
    STARTUP     = 0,
    IDLE    = 1,
    NEW_TURN    = 2,
    WIN     = 3,
    LOSE    = 4
};

class Engine
{
public:
    Engine() {}
    ~Engine();

    void render();
    void update();

    void newGame();                         //nouvelle partie
    void loadGame();                        //charge la partie en cours, il ne peut y avoir qu'une seule partie

    void lose();                            //le joueur perd
    void win();                             //le joueur gagne

    TCODRandom* randomEngine;               //classe de random initialisé sur mainSeed
    long mainSeed;                          //...
    int timeTotalRound;                     //temps total de la partie (en tour)

    EntityItem* choseOneFromItemList(   std::vector<int> plistId,
                                        const std::string& title,
                                        const EntityItemType& ptype = EntityItemType::ALL);
    std::vector<EntityItem*> choseMultiFromItemList(const std::vector<int>& plistId, const std::string& title);

    EntityFixedItem* choseOneFromItemFixedList(const std::vector<int> plistId, const std::string& title);

    void changeAllToDownLevel(const unsigned int& plevel, const unsigned int& x, const unsigned int& y);
    void changeEntityToDownLevel(Entity* pentity);

    void selectTile(int& px, int& py, const float& prange);
    EntityPnj* selectPnj(const int& pcenterX, const int& pcenterY, 
                         const std::vector<EntityPnj*> ppnjs, std::vector<Point>& path); //sélection d'un pnj dans une liste donné de façon graphique
                                                                                         //une ligne est tracé depuis pcenterXY jusqu'au pnj

    void updateTimeRound();

    EntityPlayer& getPlayer() {
        return player_;
    }
    TCOD_key_t& getMainKey() {
        return mainKey_;
    }
    Map& getMap() {
        return map_;
    }
    Gui& getGui() {
        return gui_;
    }
    EntityManager& getEntityManager() {
        return entitiesManager_;
    }
    DataManager& getDataManger() {
        return dataManager_;
    }
    AnimationManager& getAnimationManager() {
        return animationsManager_;
    }
    GameStatus& getMainStatus() {
        return mainStatus_;
    }

    void setMainStatus(const GameStatus& pstatus) {
        mainStatus_ = pstatus;
    }

    static Engine* getInstance() {
        if (!instance_)
            instance_ = new Engine();

        return instance_;
    }

    bool IsRevealMode;                          //affiche l'ensemble de la carte et des PNJs/items
    bool IsRunning;

    std::string getDataXml();

private:
    void showDevConsole();                      //affiche la console de développement
    void showHelp();                            //affiche la console d'aide
    void showMenu();                            //affiche le menu ingame
    void processKey(const TCOD_key_t& pkey);           //gestion des touches pendant le jeu

    EntityPlayer player_;                       //le joueur
    Map map_;                                   //gestion de la carte
    Gui gui_;                                   //gestion du gui
    EntityManager entitiesManager_;             //gestion des entités
    DataManager dataManager_;                   //gestion des sauvegardes
    AnimationManager animationsManager_;        //gestion des animations

    TCOD_key_t mainKey_;                        //touche appuyé durant l'update

    GameStatus mainStatus_;                     //statut actuel du jeu

    static Engine* instance_;                   //instance unique du moteur

};

#endif
