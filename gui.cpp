#include "gui.hpp"
#include "main.hpp"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * Constructeur
 */
Gui::Gui()
{
	console = new TCODConsole(INFOS_WIDTH, INFOS_HEIGHT);
}

/**
 * Destructeur 
 */
Gui::~Gui()
{
	delete console;
	messages.clearAndDelete();
}

/**
 * Rendu dans la scène
 * 	- Dessin des infos joueur
 * 	- Dessin des messages
 */
void Gui::render()
{
	console->setDefaultBackground(TCODColor::black);
	console->clear();

	//vie du joueur
	renderBar(	0, 1, 20, "Vie", 
				Engine::getInstance()->getPlayer()->destructible->hp, 
				Engine::getInstance()->getPlayer()->destructible->maxHp, 
				TCODColor::red, 
				TCODColor::blue
			 );
	
	//messages
	//TODO Rendre les message plus vieux moins lumineux
	int line 		= 0;
	float colorCoef = 0.4f;
	for (GuiMessage **it = messages.begin(); it != messages.end(); it++)
	{
		GuiMessage *mess = *it;
		console->setDefaultForeground(mess->color * colorCoef);
		console->print(MESSAGES_POS_X, line, mess->text);
		
		line++;
		
		if (colorCoef < 1.0f)
			colorCoef += 0.08f;
	}
	
	//affichage de la console du GUI sur la console principale
	TCODConsole::blit(console, 0, 0, 0, 0, TCODConsole::root, INFOS_POS_X, INFOS_POS_Y);
}

/**
 * Affichage d'une jauge
 * @param x Position de la jauge
 * @param y Position de la jauge
 * @param width Largeur de la jauge
 * @param text Texte à afficher dans la barre
 * @param value Valeur actuelle de la barre
 * @param maxValue Valeur de la barre à 100%
 * @param barColor Couleur de remplissage de value
 * @param backColor Couleur de fond
 */
void Gui::renderBar(const int& px, const int& py, const int& pwidth, const char* ptext, 
					const float& pvalue, const float& pmaxValue, const TCODColor& pbarColor, 
					const TCODColor& pbackColor)
{
	console->setDefaultBackground(pbackColor);
	console->rect(px, py, pwidth, 1, false, TCOD_BKGND_SET);
	
	float widthValue = ( (pvalue * pwidth) / pmaxValue);
	console->setDefaultBackground(pbarColor);
	console->rect(px, py, widthValue, 1, false, TCOD_BKGND_SET);
	
	console->setDefaultForeground(TCODColor::white);
	console->printEx(px, py, TCOD_BKGND_NONE, TCOD_LEFT, "%s : %i / %i", ptext, int(pvalue), int(pmaxValue));
}

/**
 * Ajoute un message à la liste
 * @param color Couleur du texte
 * @param text Texte à afficher
 */
void Gui::message(const TCODColor& pcolor, const char* ptext, ...)
{
	//construction du texte avec les paramètres
	va_list ap;
	char buff[128];
	va_start(ap, ptext);
	vsprintf(buff, ptext, ap);
	va_end(ap);
	
	//la liste est complète, suppression du premier
	if (messages.size() == MESSAGES_LINES)
	{
		GuiMessage* mess = messages.get(0);
		messages.remove(mess);
		delete mess;
	}
	
	messages.push(new GuiMessage(buff, pcolor));
}

/**
 * Vide la liste des messages
 */
void Gui::clear()
{
	messages.clearAndDelete();
}

/**
 * Constructeur
 * @param newText Message
 * @param newColor Couleur du texte
 */
GuiMessage::GuiMessage(const char* ptext, const TCODColor& pcolor)
{
	text 	= new char[strlen(ptext)];
	strcpy(text, ptext);
	color	= pcolor;
}

/**
 * Destructeur
 */
GuiMessage::~GuiMessage()
{
	delete [] text;
}
