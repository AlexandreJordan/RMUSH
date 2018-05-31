#include "level.hpp"

#include "engine.hpp"
#include "constants.hpp"
#include "tools.hpp"
#include "stairs.hpp"
#include "door.hpp"
#include "entitymanager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace pugi;

class BspListener : public ITCODBspCallback
{
private:
    Level& level_;
    int roomCount;

public:
    BspListener(Level& plevel) : level_(plevel), roomCount(0) {}

    bool visitNode(TCODBsp* node, void* userData) {
        if (node->isLeaf()) {
            //paramètre de la nouvelle pièce
            int width   = level_.rnd_->getInt(ROOM_MIN_SIZE, node->w - 2);
            int height  = level_.rnd_->getInt(ROOM_MIN_SIZE, node->h - 2);
            int posX    = level_.rnd_->getInt(node->x + 1, node->x + node->w - width - 1);
            int posY    = level_.rnd_->getInt(node->y + 1, node->y + node->h - height - 1);

            //création de la pièce
            level_.createRoom(posX, posY, posX + width - 1, posY + height - 1);

            roomCount++;
        }

        return true;
    }
};


//
// Constructeur
// pwidth Largeur du level
// pheight Hauteur du level
//
Level::Level(const int& pwidth, const int& pheight, const int& pid) :
    width_(pwidth), height_(pheight), id_(pid)
{
    rnd_        = Engine::getInstance()->randomEngine;
    showPnjsFov_    = false;
    stairUpId_      = -1;
    stairDownId_    = -1;
}

//
// Destructeur
//  Vide et libère les objets des listes
//
Level::~Level()
{
    rooms_.clearAndDelete();

    delete tcmap_;
    delete [] tiles_;
}


//
// Initialisation lors du chargement
//
//      pnode == <level>
//
// Accès à une case x,y -> map[x + y * width]
//
void Level::initLoad(const xml_node& pnode)
{
    cout << " --- LEVEL " << pnode.attribute("id").as_int() << " EN COURS DE CHARGEMENT --- " << endl;

    //propriétés du level
    width_      = pnode.attribute("width").as_int();
    height_     = pnode.attribute("height").as_int();
    id_         = pnode.attribute("id").as_int();
    stairUpId_      = pnode.attribute("stairUpId").as_int();
    stairDownId_    = pnode.attribute("stairDownId").as_int();

    //informations des tiles
    //mise à jour du tcmap_
    for (xml_node tile: pnode.children())
    {
        int x = tile.attribute("x").as_int();
        int y = tile.attribute("y").as_int();

        tiles_[x + y * width_].explored     = tile.child("explored").text().as_bool();
        tiles_[x + y * width_].isDig        = tile.child("isDig").text().as_bool();
        tiles_[x + y * width_].isFire       = tile.child("isFire").text().as_bool();
        tiles_[x + y * width_].isFall       = tile.child("isFall").text().as_bool();
        tiles_[x + y * width_].mutationAffect   = tile.child("mutationAffect").text().as_float();

        if (tiles_[x + y * width_].isDig)
            tcmap_->setProperties(x, y, true, true);
    }

    cout << "width_ : " << width_ << endl;
    cout << "height_ : " << height_ << endl;
    cout << "id_ : " << id_ << endl;
    cout << endl << endl;
}


//
// Génération du level
//  Création du tableau de Tile
//  Création de TCODMap
//
void Level::generate()
{
    tcmap_ = new TCODMap(width_, height_);
    tiles_ = new Tile[width_ * height_];
}

//
// Génération des pièces par BSP
//
void Level::generateBsp()
{
    TCODBsp bsp(0, 0, width_, height_);
    bsp.splitRecursive(rnd_, ROOM_COUNT_LEVEL, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, nullptr);
}

//
// Génération des couloirs entre les pièces
//
void Level::generateBspCorridor()
{
    Room* lastRoom  = nullptr;
    int lastX       = 0;
    int lastY       = 0;
    int i       = 0;

    for (Room* room: rooms_)
    {
        //centre de la room
        int currentX = room->x + (room->width / 2);
        int currentY = room->y + (room->height / 2);

        if (i > 0)
        {
            //centre de la lastRoom
            lastX = lastRoom->x + (lastRoom->width / 2);
            lastY = lastRoom->y + (lastRoom->height / 2);

            digRectangle(currentX, currentY, lastX, currentY);
            digRectangle(currentX, currentY, currentX, lastY);

            //createDoor(currentX, currentY);
        }

        lastRoom = room;
        i++;
    }
}

//
// Génération des items et des monstres du niveau
//
void Level::populate()
{
    //nombre d'items / pnjs sur tout le level
    unsigned int pnjCount   = TCODRandom::getInstance()->getInt(MIN_POPULATE_PNJ, MAX_POPULATE_PNJ);
    unsigned int itemCount  = TCODRandom::getInstance()->getInt(MIN_POPULATE_ITEM, MAX_POPULATE_ITEM);
    unsigned int itemFixedCount = TCODRandom::getInstance()->getInt(MIN_POPULATE_ITEMFIXED, MAX_POPULATE_ITEMFIXED);

    for (unsigned int i = 0; i < pnjCount; i++)
    {
        //sélection de la pièce
        int roomIndex = TCODRandom::getInstance()->getInt(0, rooms_.size() - 1);
        Room* room = rooms_.get(roomIndex);

        //position
        int posX = TCODRandom::getInstance()->getInt(room->x, room->x + room->width - 1);
        int posY = TCODRandom::getInstance()->getInt(room->y, room->y + room->height - 1);

        //type de pnj
        int pnjType = TCODRandom::getInstance()->getInt(0, PNJ_TYPE_COUNT - 1);

        //cout << "Creation pnj id / type : " << id_ << " / " << pnjType << "pos : " << posX << " / " << posY << endl;
        Engine::getInstance()->getEntityManager().createPnj(pnjType, posX, posY, id_);
    }

    for (unsigned int i = 0; i < itemCount; i++)
    {
        //sélection de la pièce
        int roomIndex = TCODRandom::getInstance()->getInt(0, rooms_.size() - 1);
        Room* room = rooms_.get(roomIndex);

        //position
        int posX = TCODRandom::getInstance()->getInt(room->x, room->x + room->width - 1);
        int posY = TCODRandom::getInstance()->getInt(room->y, room->y + room->height - 1);

        //type de l'item
        int itemType = TCODRandom::getInstance()->getInt(0, ITEM_TYPE_COUNT - 1);

        Engine::getInstance()->getEntityManager().createItem(itemType, posX, posY, id_);
    }

    for (unsigned int i = 0; i < itemFixedCount; i++)
    {
        //sélection de la pièce
        int roomIndex = TCODRandom::getInstance()->getInt(0, rooms_.size() - 1);
        Room* room = rooms_.get(roomIndex);

        //position
        int posX = TCODRandom::getInstance()->getInt(room->x, room->x + room->width - 1);
        int posY = TCODRandom::getInstance()->getInt(room->y, room->y + room->height - 1);

        //type de l'item
        int itemType = TCODRandom::getInstance()->getInt(0, ITEMFIXED_TYPE_COUNT - 1);

        Engine::getInstance()->getEntityManager().createFixedItem(itemType, posX, posY, id_);
    }
}

//
// Génération des escaliers pour le niveau
//
void Level::generateStairs()
{
    if (id_ > 0)
        stairUpId_ = Engine::getInstance()->getEntityManager().createFixedItem(EntityFixedItemType::STAIR_UP, rooms_.get(0)->x, rooms_.get(0)->y, id_);

    if (id_ < LEVELS_COUNT - 1)
        stairDownId_ = Engine::getInstance()->getEntityManager().createFixedItem(EntityFixedItemType::STAIR_DOWN, rooms_.get(0)->x, rooms_.get(0)->y, id_);
}

//
// Mise a jour du level
//
void Level::update()
{
    computeFov(Engine::getInstance()->getPlayer().fov);

    std::vector<EntityPnj*> pnjsList = Engine::getInstance()->getEntityManager().getPnjs(id_);
    std::vector<EntityItem*> itemsList = Engine::getInstance()->getEntityManager().getItems(id_);
    std::vector<EntityFixedItem*> fixedItemsList = Engine::getInstance()->getEntityManager().getFixedItems(id_);

    //-> Mise à jour tour par tour
    if (Engine::getInstance()->getMainStatus() == GameStatus::NEW_TURN)
    {
        //mise a jour des PNJ
        for (EntityPnj* pnj: pnjsList)
            pnj->update();

        //mise a jour des items
        for (EntityItem* item: itemsList)
            item->update();

        //mise a jour des items fixe
        for (EntityFixedItem* itemFixed: fixedItemsList)
            itemFixed->update();
    }

    //-> Mise à jour temps réel
    //mise a jour des PNJ
    /*for (EntityPnj *pnj: pnjsList)
        pnj->rtupdate();

    //mise a jour des items
    for (EntityItem *item: itemsList)
    item->rtupdate();

    //mise a jour des items fixe
    for (EntityFixedItem *itemFixed: fixedItemsList)
        itemFixed->rtupdate();*/
}

//
// Dessin du level
//
void Level::render()
{
    EntityPlayer& player                            = Engine::getInstance()->getPlayer();
    std::vector<EntityPnj*> pnjsList                = Engine::getInstance()->getEntityManager().getPnjs(id_);
    std::vector<EntityItem*> itemsList              = Engine::getInstance()->getEntityManager().getItems(id_);
    std::vector<EntityFixedItem*> fixedItemsList    = Engine::getInstance()->getEntityManager().getFixedItems(id_);

    float squaredTorchRadius                = player.fov * player.fov;
    TCODColor lightColor;

    //dessin du fond de carte
    for (int x = 0; x < width_; x++)
    {
        for (int y = 0; y < height_; y++)
        {
            if (!isInFov(x, y))
                continue;

            //
            // Données
            //
            Tile currentTile = tiles_[x + y * width_];


            //
            // Caractères
            //
            char baseChar = isWall(x, y) ? CH_WALL : CH_FLOOR;


            //
            // Couleurs
            //

            //couleur hors de portée
            TCODColor baseColor = isWall(x, y) ? C_DARKWALL : ((currentTile.isFall) ? C_DARKFALL : C_DARKGROUND);

            if (currentTile.mutationAffect > 0)
                baseColor = baseColor * TCODColor::green;

            //couleur lorsque vue
            lightColor = isWall(x, y) ? C_LIGHTWALL : ( (currentTile.isFall) ? C_LIGHTFALL : C_LIGHTGROUND);

            //distance entre la case et la source de lumière sans appliquer la racine par optimisation
            float distance = (float)((x - player.x + player.lightX) * (x - player.x + player.lightX) +
                                     (y - player.y + player.lightY) * (y - player.y + player.lightY));

            //la distance est inférieur au rayon de la torche
            //par optimisation, on n'applique pas la racine à la distance calculée
            //et on la compare au rayon de la torche au carré
            if (distance < squaredTorchRadius)
            {
                //détermination du coef d'interpolation pour la couleur
                //coef = 1.0 au centre du rayonnement
                //coef = 0.0 au rayon du rayonnement
                //
                //
                float coefInterColor = (squaredTorchRadius - distance) / squaredTorchRadius + player.lightIntensity;

                //garde le coef d'interpolation pour la couleur
                //entre 2 valeurs min / max
                //si coefInterColor < 0.0f => 0.0f
                //si coefInterColor > 1.0f => 1.0f
                //si coefInterColor compris dans l'interval on utilise sa valeur
                coefInterColor = CLAMP(0.0f, 1.0f, coefInterColor);

                //interpolation de la couleur entre baseColor / lightColor
                //avec le coef déterminé par la distance du joueur
                baseColor = TCODColor::lerp(baseColor, lightColor, coefInterColor);

                lightColor = baseColor;
                TCODConsole::root->setCharBackground(x, y, lightColor);
                TCODConsole::root->setCharForeground(x, y, TCODColor::lighterGrey);

                //affichage d'un caractère ou pas ?!
                //TCODConsole::root->setChar(x, y, baseChar);
            }
        }
    }

    //dessin des PNJ
    for (EntityPnj* pnj: pnjsList)
    {
        if (isInFov(pnj->x, pnj->y) || Engine::getInstance()->IsRevealMode)
            pnj->render();
    }

    //dessin des items
    for (EntityItem* item: itemsList)
    {
        if ( (item->isOnMap && isInFov(item->x, item->y)) || Engine::getInstance()->IsRevealMode )
            item->render();
    }

    //dessin des items fixes
    for (EntityFixedItem* itemFixed: fixedItemsList)
    {
        if (isInFov(itemFixed->x, itemFixed->y) || Engine::getInstance()->IsRevealMode)
            itemFixed->render();
    }
}

//
// Creuse un rectangle
// On met a TRUE isTransparent et isWalkable dans le TCODMap
//
// px1 Point haut / gauche
// py1 Point haut / gauche
// px2 Point haut / droit
// py2 Point haut / droit
//
void Level::digRectangle(int px1, int py1, int px2, int py2)
{
    if (!isInLevel(px1, py1) || !isInLevel(px2, py2))
        return;

    if (px2 < px1)
        Tools::swapInt(px2, px1);

    if (py2 < py1)
        Tools::swapInt(py2, py1);

    for (int x = px1; x <= px2; x++)
    {
        for (int y = py1; y <= py2; y++)
        {
            tcmap_->setProperties(x, y, true, true);
            tiles_[x + y * width_].isDig = true;
        }
    }
}

//
// Creuse un cercle
//
void Level::digCircle(const int& px, const int& py, const int& radius)
{
    //parcours de toutes cases autour du centre (en formant un rectangle)
    for (int x = px - radius; x <= px + radius; x++)
        for (unsigned int y = py - radius; y <= py + radius; y++)
        {
            if (!isInLevel(x, y))
                return;

            if (Tools::getDistance(x, y, px, py) > radius)
                continue;

            if (isWall(x, y))
                tcmap_->setProperties(x, y, true, true);
        }
}

//
// Creuse un cercle sur la carte
//
void Level::digCircleFall(const unsigned int& px, const unsigned int& py, const unsigned int& radius)
{
    if (id_ >= LEVELS_COUNT)                            //dernier niveau
        return;

    for (unsigned int x = px - radius; x <= px + radius; x++)
    {
        for (unsigned int y = py - radius; y <= py + radius; y++)
        {
            if (!isInLevel(x, y))                               //pas dans le niveau
                continue;;

            if (Tools::getDistance(x, y, px, py) > radius)      //pas dans le cercle
                continue;

            //si la case du niveau suivant est creusé
            if (Engine::getInstance()->getMap().getLevels().at(id_ + 1)->getTile(x, y).isDig)
                tiles_[x + y * width_].isFall = true;
        }
    }
}

//
//Supprime définitivement les items compris dans le cercle
//
void Level::deleteItemsInCircle(const unsigned int& px, const unsigned int& py, const float& radius, EntityItem* pexclude)
{
    for (unsigned int x = px - radius; x <= px + radius; x++)
    {
        for (unsigned int y = py - radius; y <= py + radius; y++)
        {
            //TODO - A CODER - deleteItemsInCircle
            //TCODList<EntityItem*> items = getItemsFromTile(x, y);

            /*for (EntityItem* item: items)
            {
                if (item != pexclude)
                {
                    std::cout << "delete : " << item->name << std::endl;
                    delete item;
                }
            }*/
        }
    }
}

//
//Supprime définitivement les items fixes compris dans le cercle
//
void Level::deleteFixedItemsInCircle(const unsigned int& px, const unsigned int& py, const float& radius, EntityFixedItem* pexclude)
{
    for (unsigned int x = px - radius; x <= px + radius; x++)
    {
        for (unsigned int y = py - radius; y <= py + radius; y++)
        {
            //TODO - A CODER - deleteFixedItemsInCircle
            /*EntityFixedItem* item = getFixedItemFromTile(x, y);

            if (item && (item != pexclude))
                delete item;*/
        }
    }
}


//
// Création d'un pièce
//
// pfirt Premiêre piêce créée
// px1 Point haut / gauche
// py1 Point haut / gauche
// px2 Point haut / droit
// py2 Point haut / droit
//
void Level::createRoom(int px1, int py1, int px2, int py2)
{
    digRectangle(px1, py1, px2, py2);                   //on creuse la pièce
    rooms_.push(new Room(px1, py1, (px2 - px1), (py2 - py1)));      //ajout de la pièce dans la liste
}

//
// Calcul du FOV suivant la position du joueur et le rayon de vision du joueur
//
void Level::computeFov(const float& pfovRadius)
{
    tcmap_->computeFov( Engine::getInstance()->getPlayer().x,
                        Engine::getInstance()->getPlayer().y,
                        (int)pfovRadius,
                        true,
                        FOV_RESTRICTIVE);
}

//
// Retourne TRUE si la case est dans le champ de vision du joueur. Information récupéré
// depuis le TCODMap. Si x,y sont dans le FOV du joueur, mise a jour de l'attribut "explored".
//
bool Level::isInFov(const int& px, const int& py) const
{
    if (tcmap_->isInFov(px, py))
    {
        tiles_[px + py * width_].explored = true;
        return true;
    }

    return false;
}

//
// Retourne TRUE si la case en x,y est un mur
//
bool Level::isWall(const int& px, const int& py) const
{
    return !tcmap_->isWalkable(px, py);
}

//
// Retourne True si la case en x,y a déja été vu par le joueur
//
bool Level::isExplored(const int& px, const int& py) const
{
    return tiles_[px + py * width_].explored;
}

//
// VRAI si la case ciblé est vide :
//  - ce n'est pas un mur
//  - pas de pnj dessus
//      - pas d'items fixes
//  - pas d'items
//
bool Level::canWalk(const int& px, const int& py) const
{
    if (isWall(px, py))
        return false;


    //collision avec les items
    std::vector<EntityItem*> itemsList = Engine::getInstance()->getEntityManager().getItems(px, py, id_);

    for (EntityItem* item: itemsList)
    {
        if (item->x == px && item->y == py && item->block)
            return false;
    }

    //collision avec un item fixe
    EntityFixedItem* fixedItem = Engine::getInstance()->getEntityManager().getFixedItem(px, py, id_);

    if (fixedItem)
        if (fixedItem->x == px && fixedItem->y == py && fixedItem->block)
            return false;

    return true;
}

//
// VRAI si les coordonnées sont dans le niveau
//
bool Level::isInLevel(const int& px, const int& py) const
{
    return (px >= 0 && px < width_ && py >= 0 && py < height_);
}

//
// Révèle toute la carte
// Passer toutes les tiles en explored a TRUE
//
void Level::reveal()
{
    for (int x = 0; x < width_; x++)
        for (int y = 0; y < height_; y++)
            tiles_[x + y * width_].explored = true;
}

//
// Affiche le fov de tous les PNJs
//
void Level::switchShowPnjsFov()
{
    showPnjsFov_ = !showPnjsFov_;

    std::vector<EntityPnj*> pnjsList = Engine::getInstance()->getEntityManager().getPnjs(id_);

    for (EntityPnj* pnj: pnjsList)
        pnj->showFov = showPnjsFov_;
}

//
// Crée une porte
//
/*void Level::createDoor(const int& px, const int& py)
{
    Door* item          = new Door();
    item->block         = true;
    item->chr           = '+';
    item->color         = C_ITEM_DOOR;
    item->name          = "Une porte";
    item->description       = "bah une porte";
    item->x         = px;
    item->y         = py;
    fixedItems_.push(item);
}*/

//
// Retourne les données sous forme de XML
//
std::string Level::getDataXml()
{
    std::stringstream resXml;

    resXml << "<level id=\"" << id_ << "\" width=\"" << width_ << "\" height=\"" << height_ << "\" stairUpId=\"" << stairUpId_ << "\" stairDownId=\"" << stairDownId_ << "\">" << std::endl;

    for (int x = 0; x < width_; x++)
        for (int y = 0; y < height_; y++)
        {
            Tile& tile = getTile(x, y);

            resXml << "<tile x=\"" << x  << "\" y=\"" << y << "\">" << std::endl;
            resXml << "<explored>" << tile.explored << "</explored>" << std::endl;
            resXml << "<isDig>" << tile.isDig << "</isDig>" << std::endl;
            resXml << "<isFire>" << tile.isFire << "</isFire>" << std::endl;
            resXml << "<isFall>" << tile.isFall << "</isFall>" << std::endl;
            resXml << "<mutationAffect>" << tile.mutationAffect << "</mutationAffect>" << std::endl;
            resXml << "</tile>" << std::endl;
        }

    resXml << "</level>" << std::endl;

    return resXml.str();
}

