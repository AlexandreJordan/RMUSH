#ifndef WEAPON_HPP
#define WEAPON_HPP

class EntityItem;

class Weapon : public EntityItem
{
public:
    Weapon();
    ~Weapon();

    void render();
    void update();
    void rtupdate();

    void initNew(const int&, const int&, const int&, const std::string&);
    void initLoad();

    void use(EntityMobile* powner);

    int minDamage;
    int maxDamage;
};

#endif

#include "entity.hpp"

Weapon::Weapon() :
    EntityItem(), minDamage(0), maxDamage(0)
{
}

Weapon::~Weapon()
{
}


void Weapon::initNew(const int& px, const int& py, const int& pid, const std::string& pdata)
{
    id          = pid;
    block       = false;
    chr         = 't';
    color       = C_ITEM_DAGUE;
    name        = "Une dague";
    description     = "Ca coupe !";
    x           = px;
    y           = py;
    eType       = EntityItemType::WEAPON;
}

void Weapon::initLoad()
{

}

void Weapon::use(EntityMobile* powner)
{
}

void Weapon::update()
{

}

inline void Weapon::rtupdate()
{
}

void Weapon::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
