#include "stairs.hpp"
#include "engine.hpp"

#include <sstream>
#include "constants.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     ///
///                     STAIRUP                             ///
///                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StairUp::StairUp() :
    EntityFixedItem()
{
}

StairUp::~StairUp()
{
}

void StairUp::initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink)
{
    id          = pid;
    levelId     = plevelId;
    block       = false;
    chr         = '<';
    color       = C_ITEM_STAIR;
    name        = "Escalier montant";
    description     = "Vous permet de monter a l'étage supérieur";
    x           = px;
    y           = py;
    eType       = EntityFixedItemType::STAIR_UP;
}

void StairUp::initLoad(const pugi::xml_node& pnode)
{
    EntityFixedItem::initLoad(pnode);
}

std::string StairUp::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityFixedItem::getDataXml();
    return resXml.str();
}

void StairUp::use(EntityMobile* powner)
{
    Engine::getInstance()->getGui().message(TCODColor::green, "Vous montez à l'étage supérieur.");
    Engine::getInstance()->getMap().changeLevelToUp();
}

void StairUp::update()
{
}

void StairUp::rtupdate()
{
}

void StairUp::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                     ///
///                     STAIRDOWN                           ///
///                                                     ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

StairDown::StairDown() :
    EntityFixedItem()
{
}

StairDown::~StairDown()
{
}

void StairDown::initNew(const int& px, const int& py, const int& pid, const int& plevelId, Entity* pitemLink)
{
    id          = pid;
    levelId     = plevelId;
    block       = false;
    chr         = '<';
    color       = C_ITEM_STAIR;
    name        = "Escalier descendant";
    description     = "Vous permet de descendre a l'étage inférieur";
    x           = px;
    y           = py;
    eType       = EntityFixedItemType::STAIR_DOWN;
}

void StairDown::initLoad(const pugi::xml_node& pnode)
{
    EntityFixedItem::initLoad(pnode);
}

std::string StairDown::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityFixedItem::getDataXml();
    return resXml.str();
}

void StairDown::use(EntityMobile* powner)
{
    Engine::getInstance()->getGui().message(TCODColor::green, "Vous descendez à l'étage inférieur.");
    Engine::getInstance()->getMap().changeLevelToDown();
}

void StairDown::update()
{
}

void StairDown::rtupdate()
{
}

void StairDown::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}