#include "gui.hpp"

#include "libtcod.hpp"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
//#include <wchar.h>
#include <iostream>
#include "constants.hpp"

Gui::Gui() : lifeInfo(0), maxLifeInfo(0), strengthInfo(0), dexterityInfo(0), defenseInfo(0), lightOnInfo(false),
    totalRoundInfo(0.0f), speedPlayerInfo(0.0f)
{
    console_ = new TCODConsole(INFOS_WIDTH, INFOS_HEIGHT);
}

Gui::~Gui()
{
    delete console_;
    messages_.clearAndDelete();
}

//
// Rendu dans la scêne
//  - Dessin des infos joueur
//  - Dessin des messages
//
void Gui::render()
{
    console_->setDefaultBackground(TCODColor::black);
    console_->clear();

    //TODO - A CODER - INFOS JOEUR
    //renderBar(0, 0, 20, "Vie", lifeInfo * 1.0f, maxLifeInfo * 1.0f, TCODColor::red, TCODColor::blue);

    console_->setDefaultForeground(TCODColor::white);
    console_->print(0, 1, "For : %i Dex : %i Def : %i", strengthInfo, dexterityInfo, defenseInfo);
    console_->print(0, 2, "Speed : %i", speedPlayerInfo);
    console_->print(15, 2, "Vit : %i", totalRoundInfo);
    console_->print(0, 4, "Torche : %s", lightOnInfo ? "On" : "Off");

    //messages
    int line(0);
    float colorCoef(0.4f);
    int resHeight(0);

    for (GuiMessage** it = messages_.begin(); it != messages_.end(); it++)
    {
        GuiMessage* mess = *it;
        console_->setDefaultForeground(mess->color * colorCoef);

        //afficher le message dans la console
        resHeight = console_->printRect(MESSAGES_POS_X, line, INFOS_WIDTH - MESSAGES_POS_X - 2, 2, mess->text.c_str());

        line += resHeight;

        //line++;

        if (colorCoef < 1.0f)
            colorCoef += 0.08f;
    }

    //affichage de la console du GUI sur la console principale
    TCODConsole::blit(console_, 0, 0, 0, 0, TCODConsole::root, INFOS_POS_X, INFOS_POS_Y);
}

//
// Affichage d'une jauge
// px Position de la jauge
// py Position de la jauge
// pwidth Largeur de la jauge
// ptext Texte a afficher dans la barre
// pvalue Valeur actuelle de la barre
// pmaxValue Valeur de la barre a 100%
// pbarColor Couleur de remplissage de value
// pbackColor Couleur de fond
//
void Gui::renderBar(const int& px, const int& py, const int& pwidth, const std::string& ptext,
                    const float& pvalue, const float& pmaxValue, const TCODColor& pbarColor,
                    const TCODColor& pbackColor)
{
    console_->setDefaultBackground(pbackColor);
    console_->rect(px, py, pwidth, 1, false, TCOD_BKGND_SET);

    float widthValue = ( (pvalue * pwidth) / pmaxValue);
    std::cout << "pvalue / pwidth : " << pvalue << " / " << pwidth << " || " << "pmaxValue : " << pmaxValue << std::endl;
    console_->setDefaultBackground(pbarColor);
    std::cout << "px / py : " << px << " / " << py << " || " << "widthvalue : " << widthValue << std::endl;
    console_->rect(px, py, (int)widthValue, 1, false, TCOD_BKGND_SET);

    console_->setDefaultForeground(TCODColor::white);
    console_->printEx(px, py, TCOD_BKGND_NONE, TCOD_LEFT, "%s : %i / %i", ptext.c_str(), int(pvalue), int(pmaxValue));
}

//
// Ajoute un message a la liste
// color Couleur du texte
// text Texte a afficher
//
void Gui::message(const TCODColor& pcolor, const std::string ptext, ...)
{
    //construction du texte avec les paramêtres
    va_list ap;
    char buff[128];
    va_start(ap, ptext);
    vsprintf(buff, ptext.c_str(), ap);
    va_end(ap);

    //la liste est complête, suppression du premier
    if (messages_.size() == MESSAGES_LINES)
    {
        GuiMessage* mess = messages_.get(0);
        messages_.remove(mess);
        delete mess;
    }

    messages_.push(new GuiMessage(buff, pcolor));
}

//
// Vide la liste des messages
//
void Gui::clear()
{
    messages_.clearAndDelete();
}
