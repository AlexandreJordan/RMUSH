#include "engine.hpp"

#include <iostream>
#include <sstream>

#include "constants.hpp"
#include "map.hpp"
#include "level.hpp"
#include "entitymanager.hpp"
#include "libtcod.hpp"
#include "pugixml.hpp"
#include "tools.hpp"

using namespace std;

Engine* Engine::instance_ = 0;

//
// Destructeur
//
Engine::~Engine()
{
    delete randomEngine;
}

//
// Nouvelle partie
//
void Engine::newGame()
{
    //cr?ation du moteur de nombre al?atoire et g?n?ration
    //d'une seed pour pouvoir reg?n?rer les m?mes ?l?ments
    mainSeed            = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
    randomEngine        = new TCODRandom(mainSeed, TCOD_RNG_CMWC);

    map_.initNew();
    player_.initNew(Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->x,
                    Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->y,
                    -1,
                    -1,
                    "");

    mainStatus_         = GameStatus::STARTUP;
    timeTotalRound      = 0.0f;
    IsRevealMode        = false;
    IsRunning           = true;
}

//
// Chargement de la partie en cours
//
void Engine::loadGame()
{
    dataManager_.load();
    mainSeed                        = dataManager_.getIntValueFromTag("seed");
    int currentLevel                = dataManager_.getIntValueFromTag("currentLevel");
    pugi::xml_node mapNode          = dataManager_.getNode("/gamesave/levels");
    pugi::xml_node playerNode       = dataManager_.getNode("/gamesave/player");
    pugi::xml_node pnjsNode         = dataManager_.getNode("/gamesave/pnjs");
    pugi::xml_node itemsNode        = dataManager_.getNode("/gamesave/items");
    pugi::xml_node fixedItemsNode   = dataManager_.getNode("/gamesave/fixedItems");

    randomEngine                    = new TCODRandom(mainSeed, TCOD_RNG_CMWC);

    map_.initLoad(currentLevel, mapNode);
    entitiesManager_.initLoad(pnjsNode, itemsNode, fixedItemsNode);
    player_.initLoad(playerNode);

    mainStatus_                     = GameStatus(dataManager_.getIntValueFromTag("mainStatus"));
    timeTotalRound                  = dataManager_.getIntValueFromTag("timeTotalRound");

    IsRevealMode                    = false;
    IsRunning                       = true;
}

//
// Mise à jour du jeu
//  Gestion des touches pour les fen?tres syst?me
void Engine::update()
{
    if (!animationsManager_.isEmpty())
        animationsManager_.update();

    //touche appuy?e
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &mainKey_, nullptr);

    mainStatus_ = GameStatus::IDLE;

    processKey(mainKey_);

    //mise à jour du joueur
    player_.update();
    player_.rtupdate();

    //mise à jour de la carte
    map_.update();

    //mise à jour du GUI
    gui_.lifeInfo       = player_.life;
    gui_.maxLifeInfo    = player_.maxLife;
    gui_.strengthInfo   = player_.strength;
    gui_.dexterityInfo  = player_.dexterity;
    gui_.defenseInfo    = player_.defense;
    gui_.lightOnInfo    = player_.lightOn;
    gui_.totalRoundInfo = timeTotalRound;
    gui_.weaponNameInfo = player_.getWeaponNameWield();
}

//
// Dessin du jeu
//
void Engine::render()
{
    TCODConsole::root->clear();

    map_.render();
    player_.render();
    gui_.render();

    if (!animationsManager_.isEmpty()) 
        animationsManager_.render();
}

void Engine::processKey(const TCOD_key_t& pkey)
{
    //gestion des touches syst?mes
    switch (pkey.vk)
    {
    //vide la liste des messages
    case TCODK_DELETE   :
        gui_.clear();
        break;

    //affiche l'aide
    case TCODK_F1       :
        showHelp();
        break;

    //affiche la console de d?veloppement
    case TCODK_F2       :
        showDevConsole();
        break;

    //affiche le FOV de tous les PNJs
    case TCODK_F3       :
        map_.getCurrentLevel().switchShowPnjsFov();
        break;

    //affiche toute la carte
    case TCODK_F4       :
        map_.revealCurrentLevel();
        break;

    //affiche tous les pnjs et items
    case TCODK_F5       :
        Engine::getInstance()->IsRevealMode = !Engine::getInstance()->IsRevealMode;
        break;

    //sauvegarde
    case TCODK_F9       :
        dataManager_.save();
        gui_.message(TCODColor::white, "Partie sauvegardee");
        break;

    //suppression du fichier de sauvegarde
    case TCODK_F12      :
        if (dataManager_.deleteSaveFile())
            gui_.message(TCODColor::white, "Fichier de sauvegarde supprim?");
        else
            gui_.message(TCODColor::white, "Impossible de supprimer le fichier de sauvegarde");

        break;

    case TCODK_F8       :
        IsRunning = false;
        break;

    //menu ingame
    case TCODK_ESCAPE   :
        showMenu();
        //IsRunning = false;
        break;

    default:
        break;
    }
}

//
// Change tout ce qui se trouve sur la case x,y dans le niveau inf?rieur
//  Items
//  Pnjs
//  Joueur
//
void Engine::changeAllToDownLevel(const unsigned int& plevel, const unsigned int& x, const unsigned int& y)
{
    //TODO - A CODER - changeAllToDownLevel
    /*std::cout << "plevel : " << plevel << std::endl;

    if (plevel >= LEVELS_COUNT) //dernier niveau
    return;

    Level& currentLevel = *map_.getLevels().at(plevel);
    Level& downLevel    = *map_.getLevels().at(plevel + 1);

    //pas de sol au niveau en dessous
    if (!downLevel.getTile(x, y).isDig)
    return;

    //changer les items de niveau
    TCODList<EntityItem*> items = currentLevel.getItemsFromTile(x, y);
    for (EntityItem* item: items)
    {
    currentLevel.getItemsList().remove(item);
    item->levelId = plevel + 1;
    downLevel.getItemsList().push(item);
    }

    //changer les pnjs
    EntityPnj* pnj = currentLevel.getPnjFromTile(x, y);
    if (pnj)
    {
    currentLevel.getPnjsList().remove(pnj);
    pnj->levelId = plevel + 1;
    downLevel.getPnjsList().push(pnj);
    }*/
}

//
// Change l'entit? donn? au niveau inf?rieur
//
/*void Level::changeEntityToDownLevel(Entity* pentity)
{
    if (pentity->levelId >= LEVELS_COUNT) //dernier niveau
    return;

    Level& currentLevel = *map_.getLevels().at(pentity->levelId);
    Level& downLevel    = *map_.getLevels().at(pentity->levelId + 1);

    //pas de sol au niveau en dessous
    if (!downLevel.getTile(x, y).isDig)
    return;


}*/

//
// Affiche dans une fenêtre la liste plist et retourne l'élément sélectionné par le joueur sinon nullptr
//
EntityItem* Engine::choseOneFromItemList(vector<int> plistId, const string& title, const EntityItemType& ptype)
{
    static TCODConsole console(SELECTION_WIDTH, SELECTION_HEIGHT);
    console.setDefaultBackground(TCODColor(200, 180, 50));
    console.printFrame(0, 0, SELECTION_WIDTH, SELECTION_HEIGHT, true, TCOD_BKGND_DEFAULT, title.c_str());
    console.setDefaultForeground(TCODColor::white);

    int shortcut                 = 'a';
    int posY                     = 2;
    vector<EntityItem*> itemList = entitiesManager_.getItems(plistId);
    
    for (EntityItem* item: itemList)
    {
        if (item->eType != ptype && ptype != EntityItemType::ALL)
            continue;

        console.print(2, posY, "%c - %s", shortcut, item->name.c_str());

        posY++;
        shortcut++;
    }

    TCODConsole::blit(&console, 0, 0,
                      SELECTION_WIDTH,
                      SELECTION_HEIGHT,
                      TCODConsole::root,
                      (int)(WINDOW_WIDTH - SELECTION_WIDTH) / 2,
                      (int)(WINDOW_HEIGHT - SELECTION_HEIGHT) / 2);
    TCODConsole::flush();

    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

    if (key.vk == TCODK_CHAR)
    {
        int objIndex = key.c - 'a';

        if (objIndex >= 0 && objIndex < itemList.size())
            return itemList.at(objIndex);
    }

    return nullptr;
}

//
// Affiche une boite de dialogue avec la liste des items contenu dans plistId
//  Retourne les éléments marqué d'un +
//  Item marqueur
//      Non s?lectionn?  : -
//      S?lectionn?      : +
//
vector<EntityItem*> Engine::choseMultiFromItemList(const vector<int>& plistId, const string& title)
{
    //création d'une console pour la frame
    static TCODConsole console(SELECTION_WIDTH, SELECTION_HEIGHT);
    console.setDefaultBackground(TCODColor(200, 180, 50));
    console.printFrame(0, 0, SELECTION_WIDTH, SELECTION_HEIGHT, true, TCOD_BKGND_DEFAULT, title.c_str());
    console.setDefaultForeground(TCODColor::white);

    TCOD_key_t key;
    vector<EntityItem*> result;
    bool isClosed     = false;

    while (!isClosed)
    {
        int shortcut  = 'a';
        char selector = '-';
        int posY      = 2;

        vector<EntityItem*> itemList = entitiesManager_.getItems(plistId);

        //affichage de la liste
        for (EntityItem* item: itemList)
        {
            selector = ((item->isSelected) ? '+' : '-');

            console.print(2, posY, "%c %c %s", shortcut, selector, item->name.c_str());

            posY++;
            shortcut++;
        }

        //gestion de la console
        TCODConsole::blit(&console, 0, 0,
                          SELECTION_WIDTH,
                          SELECTION_HEIGHT,
                          TCODConsole::root,
                          (int)(WINDOW_WIDTH - SELECTION_WIDTH) / 2,
                          (int)(WINDOW_HEIGHT - SELECTION_HEIGHT) / 2);
        TCODConsole::flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        //sortie de la boucle par ESCAPE
        if (key.vk == TCODK_ESCAPE)
        {
            isClosed = true;

            for (EntityItem* item: itemList)
                item->isSelected = false;
        }

        //ajout / suppression dans la liste des sélectionnés
        if (key.vk == TCODK_CHAR)
        {
            int objIndex = key.c - 'a';

            if (objIndex >= 0 && objIndex < itemList.size())
            {
                //
                //la touche est un item de la liste
                //
                EntityItem* item = itemList.at(objIndex);
                item->isSelected = !item->isSelected;
            }
        }

        //validation par ENTER
        if (key.vk == TCODK_KPENTER)
        {
            for (EntityItem* item: itemList)
            {
                if (item->isSelected)
                {
                    result.push_back(item);
                    item->isSelected = false;
                }
            }
            isClosed = true;
        }
    }

    return result;
}


//
// Affiche dans une fenêtre la liste de plist et retourne l'élément sélectionné par le joueur sinon NULL
//
EntityFixedItem* Engine::choseOneFromItemFixedList(const vector<int> plistId, const string& title)
{
    static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
    console.setDefaultBackground(TCODColor(200, 180, 50));
    console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, title.c_str());
    console.setDefaultForeground(TCODColor::white);
    vector<EntityFixedItem*> fixedItemsList = entitiesManager_.getFixedItems(plistId);

    int shortcut = 'a';
    int posY = 1;

    for (EntityFixedItem* item: fixedItemsList)
    {
        console.print(2, posY, "%c - %s", shortcut, item->name.c_str());

        posY++;
        shortcut++;
    }

    TCODConsole::blit(&console, 0, 0,
                      INVENTORY_WIDTH,
                      INVENTORY_HEIGHT,
                      TCODConsole::root,
                      (int)(WINDOW_WIDTH - INVENTORY_WIDTH) / 2,
                      (int)(WINDOW_HEIGHT - INVENTORY_HEIGHT) / 2);
    TCODConsole::flush();

    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

    if (key.vk == TCODK_CHAR)
    {
        int objIndex = key.c - 'a';

        if (objIndex >= 0 && objIndex < fixedItemsList.size())
            return fixedItemsList.at(objIndex);
    }

    return nullptr;
}


//
// Affiche la console de d?veloppement - Informations sur le jeu
//
void Engine::showDevConsole()
{
    TCODConsole console(DEV_CONSOLE_WIDTH, DEV_CONSOLE_HEIGHT);
    console.setDefaultBackground(TCODColor(200, 180, 50));
    console.printFrame(0, 0, DEV_CONSOLE_WIDTH, DEV_CONSOLE_HEIGHT, false, TCOD_BKGND_DEFAULT, "devconsole");
    console.setDefaultForeground(TCODColor::white);

    TCOD_key_t key;
    bool isClosed = false;

    //
    //boucle d'affichage
    //
    while (!isClosed)
    {
        //syst?me
        console.print(1, 2, "###  Systeme  ###");
        console.print(1, 4, "FPS : %i", TCODSystem::getFps());
        //map
        console.print(1, 10, "###  Carte  ###");
        console.print(1, 12, "Taille           : %i, %i", map_.getCurrentLevel().getWidth(), map_.getCurrentLevel().getHeight());
        console.print(1, 13, "Nombre de salles : %i", map_.getCurrentLevel().getRooms().size());
        console.print(1, 14, "Joueur position  : %i, %i", player_.x, player_.y);
        console.print(1, 15, "Level courant    : %i", map_.getCurrentLevelId());

        //items/objets
        console.print(28, 2, "###  Mobile  ###");
        console.print(28, 4, "Monstres    : %i", entitiesManager_.getPnjs(map_.getCurrentLevelId()).size());
        console.print(28, 5, "Items       : %i", entitiesManager_.getItems(map_.getCurrentLevelId()).size());
        console.print(28, 6, "Items fixes : %i", entitiesManager_.getFixedItems(map_.getCurrentLevelId()).size());

        //vars
        console.print(1, 20, "###  Vars  ###");
        console.print(1, 21, "FOV pnjs    : %s", map_.getCurrentLevel().getShowPnjsFov() ? "Vrai" : "Faux");
        console.print(1, 22, "Reveal mode : %s", IsRevealMode ? "Vrai" : "Faux");

        //gestion de la console
        TCODConsole::blit(&console, 0, 0,
                          DEV_CONSOLE_WIDTH,
                          DEV_CONSOLE_HEIGHT,
                          TCODConsole::root,
                          (int)(WINDOW_WIDTH - DEV_CONSOLE_WIDTH) / 2,
                          (int)(WINDOW_HEIGHT - DEV_CONSOLE_HEIGHT) / 2);
        TCODConsole::flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        if (key.vk == TCODK_CHAR || key.vk == TCODK_F2)
            return;
    }
}

//
// Affiche l'aide du jeu
//  - Liste des commandes
//
void Engine::showHelp()
{
    TCODConsole console(HELP_CONSOLE_WIDTH, HELP_CONSOLE_HEIGHT);
    console.setDefaultBackground(TCODColor(200, 180, 50));
    console.printFrame(0, 0, HELP_CONSOLE_WIDTH, HELP_CONSOLE_HEIGHT, true, TCOD_BKGND_DEFAULT, "Aide");
    console.setDefaultForeground(TCODColor::white);

    TCOD_key_t key;

    string help = R"(
                        --- Actions ---

    Déplacement                             : p. num
    Monter / Descendre                      : < >
    Equiper un objet                        : e
    Récupérer un item                       : g
    Déposer un item                         : d
    Utiliser un objet de l'inventaire       : a
    Utiliser un objet au sol                : u
    Se déplacer directement sur un point    : x
    Activer la lampe du joueur              : l
 
 
 
                      --- Interface ---
 
    Menu                                    : ESC
    Ouvrir l'inventaire                     : i
    Afficher la télécommande                : t
    Afficher les items d'une case           : c
    Vider la liste des messages             : delete
    Afficher le fov des ennemis             : F4
    )";

    //syst?me
    console.print(1, 3, help.c_str());

    //gestion de la console
    TCODConsole::blit(&console, 0, 0,
                      HELP_CONSOLE_WIDTH,
                      HELP_CONSOLE_HEIGHT,
                      TCODConsole::root,
                      (int)(WINDOW_WIDTH - HELP_CONSOLE_WIDTH) / 2,
                      (int)(WINDOW_HEIGHT - HELP_CONSOLE_HEIGHT) / 2);
    TCODConsole::flush();
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

    if (key.vk == TCODK_CHAR) return;
}

//
// Affiche le menu ingame
//  - Continuer
//  - Quitter (sauvegarde comprise)
//  - Quitter (suicide du personnage)
void Engine::showMenu()
{
    //cr?ation d'une console pour la frame
    static TCODConsole console(MENU_INGAME_WIDTH, MENU_INGAME_HEIGHT);

    TCOD_key_t key;
    bool isClosed       = false;
    int posMenuY        = 3;
    char selector       = '-';
    int cursor          = posMenuY;

    while (!isClosed)
    {
        console.clear();
        console.printFrame(0, 0, MENU_INGAME_WIDTH, MENU_INGAME_HEIGHT, true, TCOD_BKGND_DEFAULT, "Menu");
        console.setDefaultForeground(TCODColor::white);

        console.printEx(2, posMenuY, TCOD_BKGND_DEFAULT, TCOD_LEFT, "%c Continuer", (posMenuY == cursor) ? '>' : '-');
        console.printEx(2, posMenuY + 1, TCOD_BKGND_DEFAULT, TCOD_LEFT, "%c Sauvegarder et quitter", (posMenuY + 1 == cursor) ? '>' : '-');
        console.printEx(2, posMenuY + 2, TCOD_BKGND_DEFAULT, TCOD_LEFT, "%c Quitter et supprimer le personnage", (posMenuY + 2 == cursor) ? '>' : '-');

        //gestion de la console
        TCODConsole::blit(&console, 0, 0,
                          MENU_INGAME_WIDTH,
                          MENU_INGAME_HEIGHT,
                          TCODConsole::root,
                          static_cast<int>(WINDOW_WIDTH - MENU_INGAME_WIDTH) / 2,
                          static_cast<int>(5));
        TCODConsole::flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        switch (key.vk)
        {
            case TCODK_UP       : if (cursor > posMenuY) cursor--;      break;
            case TCODK_DOWN     : if (cursor < posMenuY + 2) cursor++;  break;
            case TCODK_ESCAPE   : isClosed = true;                      break;
            case TCODK_ENTER    : {
                if (cursor == posMenuY) {
                    // continuer
                    isClosed = true;
                } else if (cursor == posMenuY + 1) {
                    // sauvegarder et quitter
                    dataManager_.save();
                    isClosed  = true;
                    IsRunning = false;
                } else if (cursor == posMenuY + 2) {
                    // supprimer la sauvegarde et quitter
                    dataManager_.deleteSaveFile();
                    isClosed  = true;
                    IsRunning = false;
                }
                break;
            }

            default : break;
        }
    }
}

//
// Mise ? jour du temps global : Ajout d'un tour
//
void Engine::updateTimeRound()
{
    timeTotalRound++;
}

//
// Selection d'une case
//
void Engine::selectTile(int& px, int& py, const float& prange)
{
    TCOD_key_t key;
    int lastX       = px;
    int lastY       = py;
    bool isClosed   = false;

    Level& currentLevel = map_.getCurrentLevel();

    while (!isClosed)
    {
        this->render();

        //mettre en avant les case qui SONT dans le FOV et dans le RANGE (portee donnee)
        for (int mx = 0; mx < currentLevel.getWidth(); mx++)
            for (int my = 0; my < currentLevel.getHeight(); my++)
            {
                if (currentLevel.isInFov(mx, my) && !currentLevel.isWall(mx, my) && (prange >= 0 || Tools::getDistance(px, py, mx, my) <= prange))
                {
                    TCODColor col = TCODConsole::root->getCharBackground(mx, my);
                    col = col * 0.5f;
                    TCODConsole::root->setCharBackground(mx, my, col);
                }
            }

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        switch (key.vk)
        {
            case TCODK_LEFT   : lastX -= 1;                              break;
            case TCODK_RIGHT  : lastX += 1;                              break;
            case TCODK_UP     : lastY -= 1;                              break;
            case TCODK_DOWN   : lastY += 1;                              break;
            case TCODK_ENTER  : px = lastX; py = lastY; isClosed = true; break;
            case TCODK_ESCAPE : isClosed = true;                         break;

            default :
                isClosed = true;
        }

        //position du curseur
        TCODConsole::root->setCharBackground(lastX, lastY, TCODColor::white);

        TCODConsole::flush();
    }
}

//
// Sélection d'un PNJ
//
EntityPnj* Engine::selectPnj(const int& pcenterX, const int& pcenterY, vector<EntityPnj*> ppnjs, vector<Point>& path)
{
    TCOD_key_t key;
    bool isClosed            = false;
    bool isNotSelectionnable = false;
    int pnjIndex             = 0;
    int x                    = pcenterX;
    int y                    = pcenterY;
    TCODColor interColor     = TCODColor::white;

    while (!isClosed)
    {
        this->render();

        //afficher le chemin pour le premier pnj de la liste
        //dessine une ligne directe entre les deux points
        //  -> parcours de chaque case du trajet
        //      -> si une case est un obstacle
        //          -> chemin en rouge, impossible de valider
        //      -> sinon retourner le pnj concerné
        TCODLine::init(pcenterX, pcenterY, ppnjs.at(pnjIndex)->x, ppnjs.at(pnjIndex)->y);
        do
        {
            if (!map_.getCurrentLevel().canWalk(x, y)) {
                isNotSelectionnable = true;
                interColor = TCODColor::red;
            }
            
            path.push_back(Point(x, y));

            TCODColor col;
            col = TCODColor::lerp(TCODConsole::root->getCharBackground(x, y), interColor, 0.5f);
            TCODConsole::root->setCharBackground(x, y, col);
            TCODConsole::flush();
            
        } while (!TCODLine::step(&x, &y));

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        switch (key.vk)
        {
            case TCODK_TAB    : {
                //pnj suivant, retour au premier de la liste si le pnj était le dernier
                pnjIndex   = (pnjIndex >= ppnjs.size() - 1 ? 0 : pnjIndex + 1);
                interColor = TCODColor::white;
                break;
            }
            case TCODK_ENTER  : {
                if (isNotSelectionnable)
                    break;
                else
                    return ppnjs.at(pnjIndex);
            }
            case TCODK_ESCAPE :
                return nullptr;

            default :
                return nullptr;
        }

        TCODConsole::flush();
    }
}

//
// Appelé lors de la mort ou du suicide du joueur
//
void Engine::lose()
{
}

//
// Appel? lors de la victoire du joueur
//
void Engine::win()
{
}

//
// Retourne les donn?es sous forme de XML
//
string Engine::getDataXml()
{
    stringstream resXml;

    resXml << "<seed>" << mainSeed  << "</seed>" << endl;
    resXml << "<timeTotalRound>" << timeTotalRound << "</timeTotalRound>" << endl;
    resXml << "<mainStatus>" << static_cast<int>(mainStatus_) << "</mainStatus>" << endl;

    return resXml.str();
}
