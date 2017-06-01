#ifndef LEVER_HPP
#define LEVER_HPP

class EntityFixedItem;

class Lever : public EntityFixedItem
{
public:
    Lever();
    ~Lever();

    void render();
    void update();
    void rtupdate();

    void initNew(const int&, const int&, const int&, EntityFixedItem* pitemLink = nullptr);
    void initLoad();

    void use(EntityMobile* powner);

    EntityFixedItem* ItemLink;
};

#endif

#include "entity.hpp"
#include "engine.hpp"

Lever::Lever() : EntityFixedItem()
{
}

Lever::~Lever()
{
}

void Lever::initNew(const int& px, const int& py, const int& pid, EntityFixedItem* pitemLink)
{
    id          = pid;
    block       = false;
    chr         = '/';
    color       = C_ITEM_LEVER;
    name        = "Un levier";
    description     = "Active moi...";
    x           = px;
    y           = py;
    ItemLink        = pitemLink;
}

void Lever::initLoad()
{

}

void Lever::use(EntityMobile* powner)
{
    if (!ItemLink)
        Engine::getInstance()->getGui().message(TCODColor::green, "Le levier n'est relié à rien...");

    Engine::getInstance()->getGui().message(TCODColor::green, "Vous utilisez le levier.");
    ItemLink->use(powner);
}

void Lever::update()
{
}

inline void Lever::rtupdate()
{
}

void Lever::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}
