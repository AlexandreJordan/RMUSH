#ifndef MINE_HPP
#define MINE_HPP

class EntityItem;

class Mine : public EntityItem
{
public:
    Mine();
    ~Mine();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata = "");
    void initLoad(const xml_node& pnode);
    std::string getDataXml();

    void use(EntityMobile* powner);

    float rangeDestruction;             //rayon de destruction
    bool startExplosion;                //gestion de l'explosion dans le temps réel

    bool floorDestroy;                  //Vrai : déstruction du sol
};

#endif

#include "entity.hpp"
#include "engine.hpp"
#include "level.hpp"

Mine::Mine() : EntityItem(), rangeDestruction(3.0f), startExplosion(false), floorDestroy(true)
{
}

Mine::~Mine()
{
}

void Mine::initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata)
{
    id                  = pid;
    levelId             = plevelId;
    block               = false;
    chr                 = 'x';
    color               = C_ITEM_MINE;
    name                = "Une mine";
    description         = "Explosition !!!";
    x                   = px;
    y                   = py;
    rangeDestruction    = 5.0f;
    floorDestroy        = true;
    startExplosion      = false;
}

void Mine::initLoad(const xml_node& pnode)
{
    EntityItem::initLoad(pnode);
    rangeDestruction    = pnode.child("rangeDestruction").text().as_float();
    floorDestroy        = pnode.child("floorDestroy").text().as_bool();
    startExplosion      = pnode.child("startExplosion").text().as_bool();
}

std::string Mine::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityItem::getDataXml();
    resXml << "<rangeDestruction>" << std::to_string(rangeDestruction) << "</rangeDestruction>" << std::endl;
    resXml << "<floorDestroy>" << std::to_string(floorDestroy) << "</floorDestroy>" << std::endl;
    resXml << "<startExplosion>" << std::to_string(startExplosion) << "</startExplosion>" << std::endl;

    return resXml.str();
}

void Mine::use(EntityMobile* powner)
{
    Map& map = Engine::getInstance()->getMap();

    Engine::getInstance()->getGui().message(C_MESS_INFO, "La mine explose !!!");

    map.getCurrentLevel().digCircle(x, y, rangeDestruction);

    if (floorDestroy)
    {
        //détruit le sol
        map.getCurrentLevel().digCircleFall(x, y, rangeDestruction);
        //détruit les items dans le rayon
        map.getCurrentLevel().deleteItemsInCircle(x, y, rangeDestruction, this);
        //détruit les items fixes dans le rayon
        map.getCurrentLevel().deleteFixedItemsInCircle(x, y, rangeDestruction, nullptr);
    }

    startExplosion = true;
}

void Mine::update()
{

}

void Mine::rtupdate()
{
}

void Mine::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
