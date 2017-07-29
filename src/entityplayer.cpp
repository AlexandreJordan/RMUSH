#include "entityplayer.hpp"

#include "engine.hpp"
#include "level.hpp"
#include "constants.hpp"
#include "stairs.hpp"
#include "tools.hpp"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

//
// Callback utilise par l'algo de pathfinding de LIBTCOD
//
class PathFinding : public ITCODPathCallback
{
    //
    // >  0 d‚placement possible
    // == 0 d‚placement impossible
    //
    float getWalkCost(int xFrom, int yFrom, int xTo, int yTo, void* userData ) const {
        return 1.0f;
    }
};

//
// Constructeur
//
EntityPlayer::EntityPlayer() :
    EntityPnj()
{
    noise = new TCODNoise(1);
}

EntityPlayer::~EntityPlayer()
{
    delete noise;
}

//
// Initialise les donnees du joueur lors d'une nouvelle partie
//
void EntityPlayer::initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata)
{
    levelId             = plevelId;
    block               = true;
    chr                 = '@';
    color               = C_PLAYER;
    description         = "Vous...";
    name                = "kz";
    life                = 40;
    maxLife             = 40;
    strength            = 20;
    dexterity           = 12;
    defense             = 2;
    fov                 = 10;
    x                   = px;
    y                   = py;

    torchX              = 0.0f;
    lightX              = 0.0f;
    lightY              = 0.0f;
    lightOn             = true;

    weapon              = -1;

    aHeadPart           = -1;
    aBodyPart           = -1;
    aHandsPart          = -1;
    aLegsPart           = -1;
    aFootsPart          = -1;

    itemLink            = { {8, -1}, {9, -1}, {5, -1}, {6, -1} };
    itemFixedLink       = { {2, -1}, {3, -1} };

    mutationTreshold    = 0.0f;
    isFire              = false;
    fireCpt             = 0;

    noise               = new TCODNoise(1);
    
    fillInventoryDev();
}

//
// Initialise les donnees … partir du fichier de sauvegarde
//
//          pnode == <player>
//
void EntityPlayer::initLoad(const pugi::xml_node& pnode)
{
    EntityPnj::initLoad(pnode);

    block               = true;
    description         = "Vous...";
    color               = C_PLAYER;
    chr                 = '@';
    name                = "kz";
    id                  = -1;

    mutationTreshold    = pnode.child("mutationTreshold").text().as_int();
    isFire              = pnode.child("isFire").text().as_bool();
    fireCpt             = pnode.child("fireCpt").text().as_int();
    lightX              = pnode.child("lightX").text().as_int();
    lightY              = pnode.child("lightY").text().as_int();
    lightIntensity      = pnode.child("lightIntensity").text().as_float();
    lightOn             = pnode.child("lightOn").text().as_bool();
    torchX              = pnode.child("torchX").text().as_int();
    
    weapon              = pnode.child("weapon").text().as_int();
    aHeadPart           = pnode.child("aHeadPart").text().as_int();
    aBodyPart           = pnode.child("aBodyPart").text().as_int();
    aHandsPart          = pnode.child("aHandsPart").text().as_int();
    aLegsPart           = pnode.child("aLegsPart").text().as_int();
    aFootsPart          = pnode.child("aFootsPart").text().as_int();

    //telecommande
    itemLink[8]         = pnode.child("rcItem_8").text().as_int();
    itemLink[9]         = pnode.child("rcItem_9").text().as_int();
    itemLink[5]         = pnode.child("rcItem_5").text().as_int();
    itemLink[6]         = pnode.child("rcItem_6").text().as_int();
    itemFixedLink[2]    = pnode.child("rcFixedItem_2").text().as_int();
    itemFixedLink[3]    = pnode.child("rcFixedItem_3").text().as_int();

    //inventaire
    for (pugi::xml_node idNode: pnode.child("inventory").children("id")) {
        cout << "load player data, add item id : " << idNode.text().as_int() << endl;
        inventory_.push_back(idNode.text().as_int());
    }
}

//
// Rendu du joueur
//
void EntityPlayer::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}

//
// Rendu temps reel
//
void EntityPlayer::rtupdate()
{
    torchX      += 0.2f;
    float tdx   = torchX + 20.0f;
    lightX      = noise->get(&tdx)*1.5f;
    tdx         += 30.0f;
    lightY      = noise->get(&tdx)*1.5f;
    // randomize the light intensity between -0.2 and 0.2
    lightIntensity = 0.2f * noise->get(&torchX);
}

void throwingAnimation(const int& poriginX)

//
// Mise a jour des donn‚es du joueur
//
void EntityPlayer::update()
{
    Engine* engine = Engine::getInstance();
    bool newTurn = false;

    //
    // application des proprietes du joueur
    //
    if (mutationTreshold >= 1)
    {
        cout << "Nouvelle mutation ajoutées" << endl;
        mutationTreshold = 0;
    }

    if (isFire)
    {
        life -= 5;
        fireCpt--;

        isFire = (fireCpt <= 0);
    }

    //récupération de la touche d'action
    // -> touche de déplacement : move(nouvelle position)
    // -> caractère             : action
    switch (engine->getMainKey().vk)
    {
    case TCODK_KP8      : newTurn = move(x, y -1);        break;
    case TCODK_KP2      : newTurn = move(x, y + 1);       break;
    case TCODK_KP4      : newTurn = move(x - 1, y);       break;
    case TCODK_KP6      : newTurn = move(x + 1, y);       break;
    case TCODK_KP9      : newTurn = move(x + 1, y - 1);   break;
    case TCODK_KP3      : newTurn = move(x + 1, y + 1);   break;
    case TCODK_KP1      : newTurn = move(x - 1, y + 1);   break;
    case TCODK_KP7      : newTurn = move(x - 1, y - 1);   break;
    case TCODK_CHAR     : newTurn = processKey(engine->getMainKey().c); break;

    default             : break;
    }

    //une action ou un déplacement a été effectué
    // -> lancement d'un nouveau tour
    // -> mise à jour du fov
    if (newTurn)
    {
        engine->setMainStatus(GameStatus::NEW_TURN);
        engine->updateTimeRound();
        engine->getMap().getCurrentLevel().computeFov(this->fov);
    }
}


//
// Gestion des touches concernant le joueur
//
bool EntityPlayer::processKey(const int& key)
{
    Engine* engine = Engine::getInstance();
    int currentLevelId = engine->getMap().getCurrentLevelId();

    switch (key)
    {
        //
        //utilisation d'un objet de l'inventaire
        //
        case 'a' :
        {
            EntityItem* item = Engine::getInstance()->choseOneFromItemList(inventory_, "Utiliser");
            if (item) item->use(this);
            break;
        }

        //
        //equiper une arme
        //
        case 'e' :
        {
            //TODO - A CODER - inventaire
            /*EntityItem* item = Engine::getInstance()->choseOneFromItemList(inventory_, "Equiper", EntityItemType::WEAPON);

            if (item) weapon = item;
            break;*/
        }

        //
        //utilisation d'un objet au sol
        //
        case 'u' :
        {
            EntityFixedItem* itemAtGround = engine->getEntityManager().getFixedItem(x, y, currentLevelId);
            if (itemAtGround) itemAtGround->use(this);
            break;
        }

        //
        //monter un escalier
        //
        case '<' :
        {
            EntityFixedItem* itemAtGround = engine->getEntityManager().getFixedItem(x, y, currentLevelId);
            StairUp* stair = dynamic_cast<StairUp*>(itemAtGround);
            if (stair) stair->use(this);
            break;
        }

        //
        //descendre un escalier
        //
        case '>' :
        {
            EntityFixedItem* itemAtGround = engine->getEntityManager().getFixedItem(x, y, currentLevelId);
            StairDown* stair = dynamic_cast<StairDown*>(itemAtGround);
            if (stair) stair->use(this);
            break;
        }

        //
        //afficher l'inventaire
        //
        case 'i' : showInventoryPanel(); break;

        //
        //afficher la telecommande universel
        //
        case 't' : showRemotePanel(); break;

        //
        //lacher un objet
        //
        case 'd' :
        {
            vector<EntityItem*> selectedItems = engine->choseMultiFromItemList(inventory_, "D‚poser un objet");
            for (EntityItem* item: selectedItems)
                dropToGround(item->id);
            break;
        }

        //
        //recuperer un objet
        //
        case 'g' :
        {
            std::vector<int> itemsList = engine->getEntityManager().getItemsId(x, y, currentLevelId);

            if (itemsList.size() == 0)
            {
                //
                //pas d'objet au sol
                //
                engine->getGui().message(C_MESS_INFO, "Il n'y a rien au sol");
            }
            else
                if (itemsList.size() == 1)
                {
                    //
                    //recuperation de l'objet
                    //
                    if (addToInventory(itemsList.at(0)))
                        engine->getGui().message(C_MESS_INFO, "Vous r‚cup‚rez %s", engine->getEntityManager().getItem(itemsList.at(0))->name.c_str());

                }
                else
                    if (itemsList.size() > 1)
                    {
                        //
                        //afficher une boite de dialogue pour le choix multiple
                        //
                        vector<EntityItem*> selectedItems = engine->choseMultiFromItemList(itemsList, "S‚lection");

                        for (EntityItem* item: selectedItems)
                            addToInventory(item->id);
                    }

            break;
        }

        //
        //afficher les infos/items d'une case dans le FOV du joueur
        //
        case 'c' :
        {
            int x(0);
            int y(0);
            std::string line;

            int nx = x;
            int ny = y;

            Engine::getInstance()->selectTile(nx, ny, fov);

            std::vector<EntityItem*> items = engine->getEntityManager().getItems(x, y, currentLevelId);

            for (int i = 0; i < items.size() - 1; i++)
            {
                if (i != 0) line += "; ";

                line += items.at(i)->name;
            }

            EntityPnj* pnj = Engine::getInstance()->getEntityManager().getPnj(x, y, currentLevelId);

            if (pnj) line += pnj->name;

            Engine::getInstance()->getGui().message(C_MESS_INFO, line);

            break;
        }

        //
        //attaque à distance
        //
        case 'f' :
        {
            //  -> affichage du chemin du lancé
            //  -> Enter / "f"  : tire si le chemin est dépourvu d'obstacle
            //  -> Echap        : annulation du tire
            //le pnj existe bien
            //  -> début de l'animation de lancer
            //  -> calcul des dégats
            EntityPnj* pnjTarget = nullptr;

            //selection d'un pnj par le joueur
            vector<EntityPnj*> pnjs = engine->getEntityManager().getPnjs(x, y, fov, engine->getMap().getCurrentLevelId());
            if (pnjs.size() > 0)
                pnjTarget = engine->selectPnj(x, y, pnjs);

            if (!pnjTarget)
                break;

            
            //SLEEP
            //startThrowingAnimation(list<coord>, caractère)
            //CONTINUE

            attack(pnjTarget);
        }

        //
        //se deplacer directement … un endroit cilbe
        //
        case 'x' :
        {
            int nx = x;
            int ny = y;

            engine->selectTile(nx, ny, fov);

            TCODDijkstra* path = new TCODDijkstra(MAP_WIDTH, MAP_HEIGHT, new PathFinding(), nullptr);
            path->compute(x, y);
            path->setPath(nx, ny);

            while (!path->isEmpty())
            {
                int px;
                int py;

                if (path->walk(&px, &py))
                {
                    x = px;
                    y = py;
                    std::cout << "deplacement : " << px << " / " << py << std::endl;
                }
                else
                {
                    std::cout << "bloqu‚ !" << std::endl;
                }
            }

            delete path;

            break;
        }

        //
        //active/desactive la torche du joueur
        //
        case 'l' :
        {
            Engine::getInstance()->getPlayer().lightOn = !Engine::getInstance()->getPlayer().lightOn;
            break;
        }

        //
        // DEV
        //
        case 'b' :
        {
            setPosition(Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->x,
                        Engine::getInstance()->getMap().getCurrentLevel().getFirstRoom()->y);
            break;
        }

        case 'n' :
        {
            setPosition(Engine::getInstance()->getMap().getCurrentLevel().getLastRoom()->x,
                        Engine::getInstance()->getMap().getCurrentLevel().getLastRoom()->y);
            break;
        }

        default:
            break;
    }
}

bool EntityPlayer::action(const int& ptargetX, const int& ptargetY)
{
}

bool EntityPlayer::move(const int& ptargetX, const int& ptargetY)
{
    Level& lvl = Engine::getInstance()->getMap().getCurrentLevel();

    //impossible de marcher sur la case cible
    if (!lvl.canWalk(ptargetX, ptargetY))
        return false;

    //il n'y a plus de sol, le joueur tombe
    if (lvl.getTile(ptargetX, ptargetY).isFall) {
        Engine::getInstance()->getMap().changeLevelToDown();
        return false;
    }

    EntityPnj* pnj = Engine::getInstance()->getEntityManager().getPnj(ptargetX, ptargetY, lvl.getId());
    if (pnj)
    {
        if (!pnj->isDead)
        {
            //
            //pnj vivant
            //
            attack(pnj);
            return false;
        }
        else
        {
            //
            //pnj mort
            //
            Engine::getInstance()->getGui().message(TCODColor::orange, "Le corps de %s  !!!", pnj->name.c_str());
        }
    }

    //
    //il y a un item dans la case cible
    //
    std::vector<EntityItem*> itemsList = Engine::getInstance()->getEntityManager().getItems(ptargetX, ptargetY, lvl.getId());

    for (EntityItem* item: itemsList)
        Engine::getInstance()->getGui().message(TCODColor::orange, "%s est au sol", item->name.c_str());

    //
    //il y a un item fixe dans la case cible
    //
    EntityFixedItem* fixedItem = Engine::getInstance()->getEntityManager().getFixedItem(ptargetX, ptargetY, lvl.getId());

    if (fixedItem)
    {
        fixedItem->launch(this);
        Engine::getInstance()->getGui().message(TCODColor::orange, "%s", fixedItem->name.c_str());
    }

    //d‚placement effectuee mise a jour de la position du joueur
    x = ptargetX;
    y = ptargetY;

    //
    // Gestion des proprietes de la case
    //  - Mutagène
    //  - Feux
    mutationTreshold += lvl.getTile(x, y).mutationAffect;
    isFire            = lvl.getTile(x, y).isFire;

    setPosition(ptargetX, ptargetY);
    return true;
}

bool EntityPlayer::attack(EntityPnj* ppnj)
{
    //
    //pnj vivant
    //
    Engine::getInstance()->getGui().message(TCODColor::orange, "Vous attaquez %s", ppnj->name.c_str());

    //récupération des dommages de l'arme du monstre
    //random entre min et max de l'arme
    //sinon utilisation d'une valeur pour combat à main nue
    int minDamageWeapon = 0;
    int maxDamageWeapon = 5;

    /*if (weapon)
    {
        minDamageWeapon = dynamic_cast<Weapon*>(weapon)->minDamage;
        maxDamageWeapon = dynamic_cast<Weapon*>(weapon)->maxDamage;
    }*/

    //calcul des d‚gats
    ppnj->takeDamage(Tools::getDamage(strength, dexterity, minDamageWeapon, maxDamageWeapon, ppnj->dexterity, ppnj->defense));
}

//
// Calcul et applique les dommages au joueur
//
void EntityPlayer::takeDamage(const float& pdamage)
{
    if (pdamage >= life)
    {
        life    = 0;
        isDead  = true;
        chr     = '%';
    }

    life = life - pdamage;
}

//
// Ajoute l'item passe en parametre dans l'inventaire du joueur
//
bool EntityPlayer::addToInventory(const int& pitemId)
{
    EntityItem* item = Engine::getInstance()->getEntityManager().getItem(pitemId);

    if (!item)
        return false;

    if (inventory_.size() > INVENTORY_SIZE)
        return false;

    item->setPosition(-1, -1);
    item->levelId = -1;
    item->isOnMap = false;

    inventory_.push_back(pitemId);

    return true;
}

//
// Depose item au sol et le retire de l'inventaire
//
void EntityPlayer::dropToGround(const int& pitemId)
{
    EntityItem* item = Engine::getInstance()->getEntityManager().getItem(pitemId);

    if (!item)
        return;

    item->setPosition(x, y);
    item->levelId = Engine::getInstance()->getMap().getCurrentLevelId();
    item->isOnMap = true;
    
    vector<int>::iterator it = find(inventory_.begin(), inventory_.end(), item->id);
    if (it != inventory_.end())
        inventory_.erase(it);
}

//
// Affiche l'inventaire
//  - Possibilite d'utiliser un item
//  - Possibilite d'‚quiper un item
//  - Possibilite de drop un item
//  - Affiche les informations d'un item
//  - Afficher l'‚quipement du joueur
//
void EntityPlayer::showInventoryPanel()
{
    //creation d'une console pour la frame
    static TCODConsole console(INVENTORY_WIDTH, INVENTORY_HEIGHT);

    TCOD_key_t key;
    bool isClosed(false);
    int baseListY(2);
    int cursor(baseListY);
    EntityManager& em = Engine::getInstance()->getEntityManager();

    while (!isClosed)
    {
        console.clear();
        console.printFrame(0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_DEFAULT, "Inventaire");
        console.setDefaultForeground(TCODColor::white);

        int shortcut                = 'a';
        char selector               = '-';
        int posX                    = 30;
        int posY                    = baseListY;
        EntityItem* itemSelected    = nullptr;

        console.print(2, 2,  "Equipement");
        console.print(2, 6,  "Arme : %s",      (weapon != -1)     ? em.getItem(weapon)->name.c_str()     : "-");
        console.print(2, 8,  "headPart  : %s", (aHeadPart  != -1) ? em.getItem(aHeadPart)->name.c_str()  : "-");
        console.print(2, 9,  "bodyPart  : %s", (aBodyPart  != -1) ? em.getItem(aBodyPart)->name.c_str()  : "-");
        console.print(2, 10, "handsPart : %s", (aHandsPart != -1) ? em.getItem(aHandsPart)->name.c_str() : "-");
        console.print(2, 11, "legsPart  : %s", (aLegsPart  != -1) ? em.getItem(aLegsPart)->name.c_str()  : "-");
        console.print(2, 12, "footsPart : %s", (aFootsPart != -1) ? em.getItem(aFootsPart)->name.c_str() : "-");

        //affichage de la liste
        vector<EntityItem*>itemList = em.getItems(inventory_);

        for (EntityItem* item: itemList)
        {
            if (posY == cursor)
            {
                itemSelected = item;
                selector = '>';

                console.print(posX, baseListY + 14, "%s", item->name.c_str());
                console.printRect(posX, baseListY + 17, INVENTORY_WIDTH - posX - 4, 6, "%s", item->description.c_str());
            }
            else
            {
                selector = '-';
            }

            console.print(posX, posY, "%c %c   (%c) %s", selector, shortcut, item->chr, item->name.c_str());

            posY++;
            shortcut++;
        }

        console.print(2, INVENTORY_HEIGHT - 2, "(a) Utiliser - (w)Porter - (d) Jeter au sol");

        //gestion de la console
        TCODConsole::blit(&console, 0, 0,
                          INVENTORY_WIDTH,
                          INVENTORY_HEIGHT,
                          TCODConsole::root,
                          static_cast<int>(WINDOW_WIDTH - INVENTORY_WIDTH) / 2,
                          static_cast<int>(INVENTORY_POS_Y));
        TCODConsole::flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        switch (key.vk)
        {
            case TCODK_KP8 :
            {
                if (cursor > baseListY) cursor--;
                break;
            }

            case TCODK_KP2 :
            {
                if (cursor < inventory_.size() + baseListY - 1) cursor++;
                break;
            }

            case TCODK_CHAR:
            {
                switch (key.c)
                {
                    case 'a'    : if (itemSelected) itemSelected->use(this); break;
                    case 'd'    : if (itemSelected) dropToGround(itemSelected->id); break;
                    case 'w'    :  //if (itemSelected) equipItem(itemSelected); break;
                    default : break;
                }

            }

            case TCODK_ESCAPE:
            {
                isClosed = true;
                break;
            }

            default : break;
        }
    }
}

//
// Affiche l'interface de la telecommande universel (TU)
//  - Elle permet de piloter toute sorte d'item a distance
//  - 4 boutons pour item
//  - 2 boutons pour item fixe
//
void EntityPlayer::showRemotePanel()
{
    static TCODConsole console(REMOTE_WIDTH, REMOTE_HEIGHT);

    TCOD_key_t key;
    bool isClosed(false);
    int selection(8);
    EntityManager& em = Engine::getInstance()->getEntityManager();

    while (!isClosed)
    {
        console.clear();
        console.printFrame(0, 0, REMOTE_WIDTH, REMOTE_HEIGHT, true, TCOD_BKGND_DEFAULT, "Télécommande universelle");
        console.setDefaultForeground(TCODColor::white);

        //dessin des boutons
        console.setDefaultBackground(TCODColor::white);
        console.rect(4 , 4 , 17, 5 , true, TCOD_BKGND_SET); // -- 8 --
        console.rect(28, 4 , 17, 5 , true, TCOD_BKGND_SET); // -- 9 --
        console.rect(4 , 10, 17, 5 , true, TCOD_BKGND_SET); // -- 5 --
        console.rect(4 , 16, 17, 5 , true, TCOD_BKGND_SET); // -- 2 --
        console.rect(28, 10, 17, 5 , true, TCOD_BKGND_SET); // -- 6 --
        console.rect(28, 16, 17, 5 , true, TCOD_BKGND_SET); // -- 3 --
        console.setDefaultBackground(TCODColor::black);

        //affichage des slots
        console.setDefaultForeground(TCODColor::darkGrey);
        console.printEx(12, 6 , TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemLink.at(8) != -1) ? em.getItem(itemLink.at(8))->name.c_str() : "libre");
        console.printEx(36, 6 , TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemLink.at(9) != -1) ? em.getItem(itemLink.at(9))->name.c_str() : "libre");
        console.printEx(12, 12, TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemLink.at(5) != -1) ? em.getItem(itemLink.at(5))->name.c_str() : "libre");
        console.printEx(36, 12, TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemLink.at(6) != -1) ? em.getItem(itemLink.at(6))->name.c_str() : "libre");

        console.printEx(12, 18, TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemFixedLink.at(2) != -1) ? em.getFixedItem(itemFixedLink.at(2))->name.c_str() : "libre");
        console.printEx(36, 18, TCOD_BKGND_DEFAULT, TCOD_CENTER, "%s", (itemFixedLink.at(3) != -1) ? em.getFixedItem(itemFixedLink.at(3))->name.c_str() : "libre");

        console.print(4 , 4 , "8 %c", ((selection == 8) ? '<' : ' '));
        console.print(28, 4 , "9 %c", ((selection == 9) ? '<' : ' '));
        console.print(4 , 10, "5 %c", ((selection == 5) ? '<' : ' '));
        console.print(28, 10, "6 %c", ((selection == 6) ? '<' : ' '));
        console.print(4 , 16, "2 %c", ((selection == 2) ? '<' : ' '));
        console.print(28, 16, "3 %c", ((selection == 3) ? '<' : ' '));
        console.setDefaultForeground(TCODColor::white);

        console.print(2, REMOTE_HEIGHT - 2, "(a) activer - (=) Redefinir - (r) Vider");

        //gestion de la console
        TCODConsole::blit(&console, 0, 0,
                          REMOTE_WIDTH,
                          REMOTE_HEIGHT,
                          TCODConsole::root,
                          static_cast<int>(WINDOW_WIDTH - REMOTE_WIDTH) / 2,
                          static_cast<int>(REMOTE_POS_Y));
        TCODConsole::flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr, true);

        if (key.vk == TCODK_CHAR)
        {
            switch (key.c)
            {
                //
                // activer l'element lie
                //
                case 'a' :
                {
                    if (itemLink.find(selection) != itemLink.end())
                    {
                        if (itemLink.at(selection) != -1)
                        {
                            em.getItem(itemLink.at(selection))->use(this);
                            isClosed = true;
                        }
                    }
                    else if (itemFixedLink.find(selection) != itemFixedLink.end())
                    {
                        if (itemFixedLink.at(selection) -1)
                        {
                            em.getFixedItem(itemFixedLink.at(selection))->use(this);
                            isClosed = true;
                        }
                    }
                    break;
                }

                case '=' :
                {
                    if (itemLink.find(selection) != itemLink.end())
                    {
                        EntityItem* item = Engine::getInstance()->choseOneFromItemList(Engine::getInstance()->getPlayer().getInventory(), "test");
                        if (item) itemLink.at(selection) = item->id;
                    }
                    else if (itemFixedLink.find(selection) != itemFixedLink.end())
                    {
                        //recuperation des items fixes dans le fov du joueur
                        vector<EntityFixedItem*> allFixedItemsList = em.getFixedItems(Engine::getInstance()->getMap().getCurrentLevelId());
                        vector<int> fixedItemsFovList;
                        
                        for (EntityFixedItem* fixedItem: allFixedItemsList)
                        {
                            if (Engine::getInstance()->getMap().getCurrentLevel().isInFov(fixedItem->x, fixedItem->y))
                                fixedItemsFovList.push_back(fixedItem->id);
                        }
                        
                        //choix de l'item fixe
                        EntityFixedItem* item = Engine::getInstance()->choseOneFromItemFixedList(fixedItemsFovList, "Choix");

                        if (item) itemFixedLink.at(selection) = item->id;
                    }

                    break;
                }

                case 'r' :
                {
                    if (itemLink.find(selection) != itemLink.end())
                        itemLink.at(selection) = -1;
                    else if (itemFixedLink.find(selection) != itemFixedLink.end())
                        itemFixedLink.at(selection) = -1;
                    break;
                }

                default :
                    break;
            }
        }
        else
        {
            switch (key.vk)
            {
                case TCODK_KP8      : selection = 8; break;
                case TCODK_KP9      : selection = 9; break;
                case TCODK_KP5      : selection = 5; break;
                case TCODK_KP6      : selection = 6; break;
                case TCODK_KP2      : selection = 2; break;
                case TCODK_KP3      : selection = 3; break;
                case TCODK_ESCAPE   : isClosed = true; break;
                default             : break;
            }
        }
    }
}

//
// Tente d'equiper item au joueur
//
void EntityPlayer::equipItem(const int& pitemId)
{
    //TODO - A CODER - equipItem
    /*switch (item->type)
    {
        case EntityItemType::WEAPON :
        {
            weapon = item;
            break;
        }

        case EntityItemType::ARMOR_HEAD :
        {
            aHeadPart = item;
            break;
        }

        case EntityItemType::ARMOR_BODY :
        {
            aBodyPart = item;
            break;
        }

        case EntityItemType::ARMOR_HANDS :
        {
            aHandsPart = item;
            break;
        }

        case EntityItemType::ARMOR_LEGS :
        {
            aLegsPart = item;
            break;
        }

        case EntityItemType::ARMOR_FOOTS :
        {
            aFootsPart = item;
            break;
        }

        default:
            break;
    }*/
}

//
// Retourne les donnees sous forme de XML
//
std::string EntityPlayer::getDataXml()
{
    std::stringstream resXml;

    resXml << "<player>" << std::endl;
    resXml << EntityPnj::getDataXml();

    //proprietes
    resXml << "<mutationTreshold>" << mutationTreshold << "</mutationTreshold>" << std::endl;
    resXml << "<isFire>" << isFire << "</isFire>" << std::endl;
    resXml << "<fireCpt>" << fireCpt << "</fireCpt>" << std::endl;
    resXml << "<lightX>" << lightX << "</lightX>" << std::endl;
    resXml << "<lightY>" << lightY << "</lightY>" << std::endl;
    resXml << "<lightIntensity>" << lightIntensity << "</lightIntensity>" << std::endl;
    resXml << "<lightOn>" << lightOn << "</lightOn>" << std::endl;
    resXml << "<torchX>" << torchX << "</torchX>" << std::endl;

    //equipement
    resXml << "<weapon>" << weapon << "</weapon>" << std::endl;
    resXml << "<aHeadPart>" << aHeadPart << "</aHeadPart>" << std::endl;
    resXml << "<aBodyPart>" << aBodyPart << "</aBodyPart>" << std::endl;
    resXml << "<aLegsPart>" << aLegsPart << "</aLegsPart>" << std::endl;
    resXml << "<aFootsPart>" << aFootsPart << "</aFootsPart>" << std::endl;
    resXml << "<aHandsPart>" << aHandsPart << "</aHandsPart>" << std::endl;

    //inventaire
    resXml << "<inventory>" << std::endl;
    for (int id: inventory_)
      resXml << "<id>" << id << "</id>" << std::endl;
    resXml << "</inventory>" << std::endl;

    //telecommande
    resXml << "<rcItem_8>" << itemLink[8] << "</rcItem_8>" << std::endl;
    resXml << "<rcItem_9>" << itemLink[9] << "</rcItem_9>" << std::endl;
    resXml << "<rcItem_5>" << itemLink[5] << "</rcItem_5>" << std::endl;
    resXml << "<rcItem_6>" << itemLink[6] << "</rcItem_6>" << std::endl;
    resXml << "<rcFixedItem_2>" << itemFixedLink[2] << "</rcFixedItem_2>" << std::endl;
    resXml << "<rcFixedItem_3>" << itemFixedLink[3] << "</rcFixedItem_3>" << std::endl;

    resXml << "</player>" << std::endl;

    return resXml.str();
}

//
// DEV
// Rempli l'inventaire pour les tests
//
void EntityPlayer::fillInventoryDev()
{
    EntityManager& em = Engine::getInstance()->getEntityManager();
    
    //ajout d'une mine dans l'inventaire
    int newId = em.createItem(EntityItemType::MINE, 0, 0, 0);
    inventory_.push_back(newId);
    em.getItem(newId)->isOnMap = false;    
    
    //ajout d'un bandage dans l'inventaire
    newId = em.createItem(EntityItemType::BANDAGE, 0, 0, 0);
    inventory_.push_back(newId);
    em.getItem(newId)->isOnMap = false;
}
