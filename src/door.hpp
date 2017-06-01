#ifndef DOOR_HPP
#define DOOR_HPP

class EntityFixedItem;

class Door : public EntityFixedItem
{
public:
    Door();
    ~Door();

    void render();
    void update();
    void rtupdate();

    void initNew(const int&, const int&, const int&, Entity* pitemLink);
    void initLoad();

    void use(EntityMobile* powner);
};

#endif

#include "entity.hpp"
#include "engine.hpp"

Door::Door() :
    EntityFixedItem()
{
}

Door::~Door()
{
}

void Door::initNew(const int& px, const int& py, const int& pid, Entity* pitemLink)
{

}

void Door::initLoad()
{

}

void Door::use(EntityMobile* powner)
{
    block = !block;
    Engine::getInstance()->getGui().message(TCODColor::green, "La porte %s", (block ? " se ferme" : " s'ouvre" ));
}

void Door::update()
{
}

inline void Door::rtupdate()
{
}

void Door::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
