#ifndef Gui_HPP
#define Gui_HPP

#include "libtcod.hpp"
#include <string>

struct GuiMessage
{
    std::string text;
    TCODColor color;

    GuiMessage(const std::string& ptext, const TCODColor& pcolor) :
        text(ptext), color(pcolor) {}
    ~GuiMessage() {}
};


class Gui
{
public:
    Gui();
    ~Gui();

    void render();
    void message(const TCODColor& pcolor, const std::string ptext, ...);
    void clear();

    int lifeInfo;
    int maxLifeInfo;
    int strengthInfo;
    int dexterityInfo;
    int defenseInfo;
    bool lightOnInfo;

    float totalRoundInfo;
    float speedPlayerInfo;

private:
    TCODConsole* console_;

    void renderBar(const int& px, const int& py, const int& pwidth, const std::string& ptext,
                   const float& pvalue, const float& pmaxValue, const TCODColor& pbarColor,
                   const TCODColor& pbackColor);

    TCODList<GuiMessage*> messages_;

};

#endif
