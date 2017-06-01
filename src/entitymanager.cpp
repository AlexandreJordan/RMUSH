#include "entitymanager.hpp"

#include <iostream>
#include "constants.hpp"
#include "bandage.hpp"
#include "mine.hpp"
#include "gaztrap.hpp"
#include "lever.hpp"
#include "weapon.hpp"
#include "armor.hpp"
#include "pitfall.hpp"
#include "stairs.hpp"

using namespace std;

//
// Constructeur
//
EntityManager::EntityManager()
{
    //TODO - A CODER - nouvelle partie : 0 / chargement : récupérer le dernier lastid
    lastId_ = 0;
}

//
// Destructeur
//
EntityManager::~EntityManager()
{
    for (pair<int, EntityPnj*> pnj: pnjs_)
        delete pnj.second;

    for (pair<int, EntityItem*> item: items_)
        delete item.second;

    for (pair<int, EntityFixedItem*> fixeditem: fixedItems_)
        delete fixeditem.second;

    pnjs_.clear();
    items_.clear();
    fixedItems_.clear();
}

//
// Chargement des données lors d'une partie en cours
//
//      ppnjsNode == <pnjs>
//      pitemsNode == <items>
//      pfixedItemsNode == <fixedItems>
//
void EntityManager::initLoad(const pugi::xml_node& ppnjsNode, const pugi::xml_node& pitemsNode, const pugi::xml_node& pfixedItemsNode)
{
    //chargement des pnjs
    for (pugi::xml_node pnjNode: ppnjsNode.children("pnj"))
        loadPnj(pnjNode);

    //chargement des items
    for (pugi::xml_node itemNode: pitemsNode.children("item"))
        loadItem(itemNode);

    //chargement des fixedItems
    for (pugi::xml_node fixedItemNode: pfixedItemsNode.children("fixedItem"))
        loadFixedItem(fixedItemNode);
}

//
// Retourne le PNJ pid
//
EntityPnj* EntityManager::getPnj(const int& pid)
{
    map<int, EntityPnj*>::iterator it;
    it = pnjs_.find(pid);

    if (it != pnjs_.end())
        return it->second;
    else
        return nullptr;
}

//
// Retourne l'item pid
//
EntityItem* EntityManager::getItem(const int& pid)
{
    map<int, EntityItem*>::iterator it;
    it = items_.find(pid);

    if (it != items_.end())
        return it->second;
    else
        return nullptr;
}

//
// Retourne l'item fixe pid
//
EntityFixedItem* EntityManager::getFixedItem(const int& pid)
{
    map<int, EntityFixedItem*>::iterator it;
    it = fixedItems_.find(pid);

    if (it != fixedItems_.end())
        return it->second;
    else
        return nullptr;
}

//
// Retourne le PNJ à la position et au niveau donné
//
EntityPnj* EntityManager::getPnj(const int& px, const int& py, const int& plevelId)
{
    for (pair<int, EntityPnj*> pnj: pnjs_)
    {
        if (pnj.second->x == px && pnj.second->y == py && pnj.second->levelId == plevelId)
            return pnj.second;
    }

    return nullptr;
}

//
// Retourne l'item fixe à la position et au niveau donné
//
EntityFixedItem* EntityManager::getFixedItem(const int& px, const int& py, const int& plevelId)
{
    for (pair<int, EntityFixedItem*> fixedItem: fixedItems_)
    {
        if (fixedItem.second->x == px && fixedItem.second->y == py && fixedItem.second->levelId == plevelId)
            return fixedItem.second;
    }

    return nullptr;
}

//
// Retourne une liste d'item à la position et au niveau donné
//
vector<EntityItem*> EntityManager::getItems(const int& px, const int& py, const int& plevelId)
{
    vector<EntityItem*> list;

    for (pair<int, EntityItem*> item: items_)
    {
        if (item.second->x == px && item.second->y == py && item.second->levelId == plevelId)
            list.push_back(item.second);
    }

    return list;
}

//
// Retourne une liste d'id d'items à la position et au niveau donné
//
vector<int> EntityManager::getItemsId(const int& px, const int& py, const int& plevelId)
{
    vector<int> list;

    for (pair<int, EntityItem*> item: items_)
    {
        if (item.second->x == px && item.second->y == py && item.second->levelId == plevelId)
            list.push_back(item.first);
    }

    return list;
}


//
// Retourne une liste de pnj fixe à la position et au niveau donné
//
vector<EntityPnj*> EntityManager::getPnjs(const int& plevelId)
{
    vector<EntityPnj*> list;

    for (pair<int, EntityPnj*> pnj: pnjs_)
    {
        if (pnj.second->levelId == plevelId)
            list.push_back(pnj.second);
    }

    return list;
}

//
// Retourne une liste d'items au niveau donné
//
vector<EntityItem*> EntityManager::getItems(const int& plevelId)
{
    vector<EntityItem*> list;

    for (pair<int, EntityItem*> item: items_)
    {
        if (item.second->levelId == plevelId)
            list.push_back(item.second);
    }

    return list;
}

//
// Retourne une liste d'items suivant une liste d'identifiant
//
vector<EntityItem*> EntityManager::getItems(const vector<int>& plistId)
{
    vector<EntityItem*> result;

    for (int id: plistId)
    {
        EntityItem* item = getItem(id);

        if (item)
            result.push_back(item);
    }

    return result;
}

//
// Retourne la liste des items fixe au niveau donnné
//
vector<EntityFixedItem*> EntityManager::getFixedItems(const int& plevelId)
{
    vector<EntityFixedItem*> result;

    for (pair<int, EntityFixedItem*> fixedItem: fixedItems_)
    {
        if (fixedItem.second->levelId == plevelId)
            result.push_back(fixedItem.second);
    }

    return result;
}

//
// Retourne la liste des items fixe suivant une liste d'ID
//
vector<EntityFixedItem*> EntityManager::getFixedItems(const vector<int>& plistId)
{
    vector<EntityFixedItem*> result;

    for (int fixedItemId: plistId)
    {
        map<int, EntityFixedItem*>::iterator it;
        it = fixedItems_.find(fixedItemId);
        
        if (it != fixedItems_.end())
            result.push_back(it->second);
    }

    return result;
}

//
// Retourne un nouvel ID disponible
//
int EntityManager::getNewId()
{
    return ++lastId_;
}

//
// Création d'un pnj suivant ppnjType
//
int EntityManager::createPnj(const int& ppnjType, const int& px, const int& py, const int& plevelId)
{
    switch (ppnjType)
    {
    case EntityPnjType::HUMAN :
    {
        //EntityPnj* pnj = new EntityPnj();
    }

    default :
        return -1;
    }

    return -1;
}

//
// Création d'un item suivant pentityType
//
int EntityManager::createItem(const int& pentityType, const int& px, const int& py, const int& plevelId)
{
    switch (pentityType)
    {
        case EntityItemType::BANDAGE:
        {
            EntityItem* item = new Bandage();
            item->initNew(px, py, getNewId(), plevelId, "");
            items_.insert(pair<int, EntityItem*>(item->id, item));
            return item->id;
        }
        case EntityItemType::MINE:
        {
            EntityItem* item = new Mine();
            item->initNew(px, py, getNewId(), plevelId, "");
            items_.insert(pair<int, EntityItem*>(item->id, item));
            return item->id;
        }

        default :
            return -1;
    }

    return -1;
}

//
// Création d'un item fixe suivant pentityType
//
int EntityManager::createFixedItem(const int& pentityType, const int& px, const int& py, const int& plevelId)
{
    switch (pentityType)
    {
    case EntityFixedItemType::GAZTRAP :
    {
        EntityFixedItem* fixedItem = new GazTrap();
        fixedItem->initNew(px, py, getNewId(), plevelId);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
        return fixedItem->id;
    }

    case EntityFixedItemType::STAIR_UP :
    {
        EntityFixedItem* fixedItem = new StairUp();
        fixedItem->initNew(px, py, getNewId(), plevelId, nullptr);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
        return fixedItem->id;
    }

    case EntityFixedItemType::STAIR_DOWN :
    {
        EntityFixedItem* fixedItem = new StairDown();
        fixedItem->initNew(px, py, getNewId(), plevelId, nullptr);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
        return fixedItem->id;
    }

    default :
        return -1;
    }

    return -1;
}

void EntityManager::loadPnj(const pugi::xml_node& pnode)
{
}

void EntityManager::loadItem(const pugi::xml_node& pnode)
{
    cout << "loadItem name : " << pnode.child("name").text().as_string() << " / id : " << pnode.child("id").text().as_int() << endl;
    
    switch (EntityItemType(pnode.child("type").text().as_int()))
    {
        case EntityItemType::BANDAGE:
        {
            EntityItem* item = new Bandage();
            item->initLoad(pnode);
            items_.insert(pair<int, EntityItem*>(item->id, item));
            break;
        }
        case EntityItemType::MINE:
        {
            EntityItem* item = new Mine();
            item->initLoad(pnode);
            items_.insert(pair<int, EntityItem*>(item->id, item));
            break;
        }

        default :
            break;
    }
}

void EntityManager::loadFixedItem(const pugi::xml_node& pnode)
{
    switch (EntityFixedItemType(pnode.child("type").text().as_int()))
    {
    case EntityFixedItemType::GAZTRAP :
    {
        EntityFixedItem* fixedItem = new GazTrap();
        fixedItem->initLoad(pnode);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
    }

    case EntityFixedItemType::STAIR_UP :
    {
        EntityFixedItem* fixedItem = new StairUp();
        fixedItem->initLoad(pnode);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
    }

    case EntityFixedItemType::STAIR_DOWN :
    {
        EntityFixedItem* fixedItem = new StairDown();
        fixedItem->initLoad(pnode);
        fixedItems_.insert(pair<int, EntityFixedItem*>(fixedItem->id, fixedItem));
    }

    default :
        break;
    }
}

//
// Retourne les données globale sous forme de XML
//
string EntityManager::getDataXml()
{
    stringstream sstr;

    sstr << "<pnjs>" << endl << getDataXmlPnjs() << endl << "</pnjs>" << endl;
    sstr << "<items>" << endl << getDataXmlItems() << endl << "</items>" << endl;
    sstr << "<fixedItems>" << endl << getDataXmlFixedItems() << endl << "</fixedItems>" << endl;

    return sstr.str();
}

//
// Retourne les données des PNJS sous forme de XML
//
string EntityManager::getDataXmlPnjs()
{
    stringstream sstr;

    for (pair<int, EntityPnj*> pnj: pnjs_)
        sstr << "<pnj>" << endl << pnj.second->getDataXml() << "</pnj>" << endl;

    return sstr.str();
}

//
// Retourne les données des Items sous forme de XML
//
string EntityManager::getDataXmlItems()
{
    stringstream sstr;

    for (pair<int, EntityItem*> item: items_)
        sstr << "<item>" << endl << item.second->getDataXml() << "</item>" << endl;

    return sstr.str();
}

//
// Retourne les données des FixedItems sous forme de XML
//
string EntityManager::getDataXmlFixedItems()
{
    stringstream sstr;

    for (pair<int, EntityFixedItem*> fixedItem: fixedItems_)
        sstr << "<fixedItem>" << endl << fixedItem.second->getDataXml() << "</fixedItem>" << endl;

    return sstr.str();
}
