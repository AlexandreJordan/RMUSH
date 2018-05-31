#include "weapon.hpp"

Weapon::Weapon() : EntityItem(), minDamage(0), maxDamage(0)
{
}

Weapon::~Weapon()
{
}


void Weapon::initNew(const int& px, const int& py, const int& pid, const int& plevelId, const std::string& pdata)
{
    id          = pid;
    levelId     = plevelId;
    block       = false;
    chr         = 't';
    color       = TCODColor::red;
    name        = "Une dague";
    description = "Ca coupe !";
    x           = px;
    y           = py;
    eType       = EntityItemType::WEAPON;
    minDamage   = 5;
    maxDamage   = 10;
}

void Weapon::initLoad(const pugi::xml_node& pnode)
{
    EntityItem::initLoad(pnode);
    minDamage    = pnode.child("minDamage").text().as_int();
    maxDamage    = pnode.child("maxDamage").text().as_int();
}

std::string Weapon::getDataXml()
{
    std::stringstream resXml;

    resXml << EntityItem::getDataXml();
    resXml << "<minDamage>" << std::to_string(minDamage) << "</minDamage>" << std::endl;
    resXml << "<maxDamage>" << std::to_string(maxDamage) << "</maxDamage>" << std::endl;

    return resXml.str();
}


void Weapon::use(EntityPnj* powner)
{
}

void Weapon::update()
{

}

void Weapon::rtupdate()
{
}

void Weapon::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
