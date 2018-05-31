#ifndef ARMOR_HPP
#define ARMOR_HPP

class EntityItem;

class Armor : public EntityItem
{
public:
    Armor();
    ~Armor();

    void render();
    void update();
    void rtupdate();

    void initNew(const int&, const int&, const int&, const std::string&);
    void initLoad();

    void use(EntityPnj* powner);

    int classArmor;
};

#endif

#include "entity.hpp"

Armor::Armor() :
    EntityItem(), classArmor(0)
{
}

Armor::~Armor()
{
}

void Armor::initNew(const int& px, const int& py, const int& pid, const std::string& pdata)
{

}

void Armor::initLoad()
{

}

void Armor::use(EntityPnj* powner)
{
}

void Armor::update()
{

}

inline void Armor::rtupdate()
{
}

void Armor::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
