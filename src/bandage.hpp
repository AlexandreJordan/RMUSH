#ifndef BANDAGE_HPP
#define BANDAGE_HPP

#include "pugixml.hpp"
#include <string>
#include <sstream>

using namespace pugi;

class EntityItem;
class EntityMobile;

class Bandage : public EntityItem
{
public:
    Bandage();
    ~Bandage();

    void render();
    void update();
    void rtupdate();

    void initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata = "");
    void initLoad(const xml_node& pnode);

    std::string getDataXml();

    void use(EntityMobile* powner);

    int lifeRestore;
};

#endif

#include "entity.hpp"
#include "engine.hpp"

Bandage::Bandage() : EntityItem(), lifeRestore(0)
{
}

Bandage::~Bandage()
{
}

void Bandage::initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata)
{
    id          = pid;
    levelId     = plevelId;
    block       = false;
    chr         = 'm';
    color       = C_ITEM_BANDAGE;
    name        = "Bandage";
    description = "Bandage de soins. Arrˆte les saignements.";
    x           = px;
    y           = py;
    lifeRestore = 10;
    eType       = EntityItemType::BANDAGE;
}

void Bandage::initLoad(const xml_node& pnode)
{
    EntityItem::initLoad(pnode);
    lifeRestore = pnode.child("lifeRestore").text().as_int();
}

std::string Bandage::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityItem::getDataXml();
    resXml << "<lifeRestore>" << std::to_string(lifeRestore) << "</lifeRestore>" << std::endl;

    return resXml.str();
}

void Bandage::use(EntityMobile* powner)
{
    Engine::getInstance()->getGui().message(TCODColor::orange, "Vous utiliser un bandage.");
}

void Bandage::update()
{
}

void Bandage::rtupdate()
{
}

void Bandage::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
