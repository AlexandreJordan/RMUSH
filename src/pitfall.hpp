#ifndef PITFALL_HPP
#define PITFALL_HPP

class EntityFixedItem;

class Pitfall : public EntityFixedItem
{
public:
    Pitfall();
    ~Pitfall();

    void render();
    void update();
    void rtupdate();

    void initNew(const int&, const int&, const int&, Entity* pitemLink = nullptr);
    void initLoad();

    void use(EntityMobile* powner);
    void launch(EntityMobile* powner);
};

#endif

#include "entity.hpp"
#include "engine.hpp"

Pitfall::Pitfall() :
    EntityFixedItem()
{
}

Pitfall::~Pitfall()
{
}

void Pitfall::initNew(const int& px, const int& py, const int& pid, Entity* pitemLink)
{
    id          = pid;
    block       = false;
    chr         = 'f';
    color       = C_ITEM_PITFALL;
    name        = "Une trappe";
    description     = "la jolie trappe";
    x           = px;
    y           = py;
}

void Pitfall::initLoad()
{

}

void Pitfall::use(EntityMobile* powner)
{
}

void Pitfall::update()
{
}

inline void Pitfall::rtupdate()
{
}

void Pitfall::render()
{
    TCODConsole::root->setChar(x, y, chr);
    TCODConsole::root->setCharForeground(x, y, color);
}

//
//Déclenchement automatique
//
void Pitfall::launch(EntityMobile* ptarget)
{
    Engine::getInstance()->getGui().message(C_MESS_HIT, "Une trappe se déclenche sous vos pieds");

    Engine::getInstance()->changeAllToDownLevel(levelId, x, y);

    if (ptarget == &Engine::getInstance()->getPlayer())
        Engine::getInstance()->getMap().changeLevelToDown();
}
