#include "entity.hpp"
#include "pugixml.hpp"

#include <sstream>

using namespace std;
using namespace pugi;

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                                 ENTITY                                   ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////

//
// Constructeur
//
Entity::Entity() : x(0), y(0), name(""), description(""), block(false), chr(' '),
    color(TCODColor::white), speed(100), energy(0), state(EntityState::WAIT), levelId(0), id(-1)
{
}

//
// Modifie la position de l'entité
//
void Entity::setPosition(const int& px, const int& py)
{
    x = px;
    y = py;
}


//
// Chargement des données de l'entité contenu dans pnode
//      pnode == <entity>
//
void Entity::initLoad(const xml_node& pnode)
{
    id          = pnode.child("id").text().as_int();
    x           = pnode.child("x").text().as_int();
    y           = pnode.child("y").text().as_int();
    levelId     = pnode.child("levelId").text().as_int();
    name        = pnode.child("name").text().as_string();
    description = pnode.child("description").text().as_string();
    block       = pnode.child("block").text().as_bool();
    chr         = pnode.child("chr").text().as_int();
    speed       = pnode.child("speed").text().as_int();
    energy      = pnode.child("energy").text().as_int();
    state       = EntityState(pnode.child("state").text().as_int());
    color.setValue(pnode.child("color").text().as_float());
}

//
// Retourne les données de l'entité sous forme de XML
//
string Entity::getDataXml()
{
    stringstream resXml;

    resXml << "<id>" << id << "</id>" << endl;
    resXml << "<x>" << x << "</x>" << endl;
    resXml << "<y>" << y << "</y>" << endl;
    resXml << "<name>" << name << "</name>" << endl;
    resXml << "<description>" << description << "</description>" << endl;
    resXml << "<block>" << block << "</block>" << endl;
    resXml << "<chr>" << chr << "</chr>" << endl;
    resXml << "<color>" << color.getValue() << "</color>" << endl;
    resXml << "<speed>" << speed << "</speed>" << endl;
    resXml << "<energy>" << energy << "</energy>" << endl;
    resXml << "<state>" << static_cast<int>(state) << "</state>" << endl;
    resXml << "<levelId>" << levelId << "</levelId>" << endl;

    return resXml.str();
}

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                              ENTITYMOBILE                                ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////

//
// Chargement des données de l'entité contenu dans pnode
//
void EntityMobile::initLoad(const xml_node& pnode)
{
    Entity::initLoad(pnode);

    life        = pnode.child("life").text().as_int();
    maxLife     = pnode.child("maxLife").text().as_int();
    defense     = pnode.child("defense").text().as_int();
    strength        = pnode.child("strength").text().as_int();
    dexterity       = pnode.child("dexterity").text().as_int();
    stealth     = pnode.child("stealth").text().as_int();
    isDead      = pnode.child("isDead").text().as_bool();
    moveSpeed       = pnode.child("moveSpeed").text().as_int();
    fov         = pnode.child("fov").text().as_int();
    eType       = EntityPnjType(pnode.child("type").text().as_int());
}

//
// Retourne les données de l'entité mobile sous forme de XML
//
string EntityMobile::getDataXml()
{
    stringstream resXml;

    resXml << Entity::getDataXml();

    resXml << "<life>" << life << "</life>" << endl;
    resXml << "<maxLife>" << maxLife << "</maxLife>" << endl;
    resXml << "<defense>" << defense << "</defense>" << endl;
    resXml << "<strength>" << strength << "</strength>" << endl;
    resXml << "<dexterity>" << dexterity << "</dexterity>" << endl;
    resXml << "<stealth>" << stealth << "</stealth>" << endl;
    resXml << "<isDead>" << isDead << "</isDead>" << endl;
    resXml << "<moveSpeed>" << moveSpeed << "</moveSpeed>" << endl;
    resXml << "<fov>" << fov << "</fov>" << endl;
    resXml << "<type>" << static_cast<int>(eType) << "</type>" << endl;

    return resXml.str();
}

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                               ENTITYITEM                                 ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////

void initNew(const int&, const int&, const int&, const int&, const std::string&)
{

}

//
// Chargement des données de l'entité contenu dans pnode
//
void EntityItem::initLoad(const xml_node& pnode)
{
    Entity::initLoad(pnode);

    eType   = EntityItemType(pnode.child("type").text().as_int());
    isOnMap = pnode.child("isOnMap").text().as_bool();
}

//
// Retourne les données de l'entité mobile sous forme de XML
//
string EntityItem::getDataXml()
{
    stringstream resXml;

    resXml << Entity::getDataXml();
    resXml << "<type>" << static_cast<int>(eType) << "</type>" << endl;
    resXml << "<isOnMap>" << isOnMap << "</isOnMap>" << endl;

    return resXml.str();
}

////////////////////////////////////////////////////////////////////////////////
///                                                                          ///
///                            ENTITYFIXEDITEM                               ///
///                                                                          ///
////////////////////////////////////////////////////////////////////////////////

//
// Chargement des données de l'entité contenu dans pnode
//
void EntityFixedItem::initLoad(const xml_node& pnode)
{
    Entity::initLoad(pnode);

    eType = EntityFixedItemType(pnode.child("type").text().as_int());
}

//
// Retourne les données de l'entité mobile sous forme de XML
//
string EntityFixedItem::getDataXml()
{
    stringstream resXml;

    resXml << Entity::getDataXml();
    resXml << "<type>" << static_cast<int>(eType) << "</type>" << endl;

    return resXml.str();
}
