#include "gui.hpp"
#include "engine.hpp"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

/**
 * Constructeur
 */
Gui::Gui()
{
	console_ = new TCODConsole(INFOS_WIDTH, INFOS_HEIGHT);
}

/**
 * Destructeur 
 */
Gui::~Gui()
{
	delete console_;
	messages_.clearAndDelete();
}

/**
 * Rendu dans la scène
 * 	- Dessin des infos joueur
 * 	- Dessin des messages
 */
void Gui::render()
{
	console_->setDefaultBackground(TCODColor::black);
	console_->clear();

	//informations joueur
	renderBar(	0, 0, 20, "Vie", 
				Engine::getInstance()->getPlayer().life, 
				Engine::getInstance()->getPlayer().maxLife, 
				TCODColor::red, 
				TCODColor::blue
			 );
	console_->print(0, 1, "Force : %f", 10.0f);
	console_->print(0, 2, "Dexterité : %f", 10.0f);
	console_->print(0, 3, "Furtivité : %f", 10.0f);
	
	//messages
	int line 		= 0;
	float colorCoef = 0.4f;
	for (GuiMessage **it = messages_.begin(); it != messages_.end(); it++)
	{
		GuiMessage *mess = *it;
		console_->setDefaultForeground(mess->color * colorCoef);
		console_->print(MESSAGES_POS_X, line, mess->text.c_str());
		
		line++;
		
		if (colorCoef < 1.0f)
			colorCoef += 0.08f;
	}
	
	//affichage de la console du GUI sur la console principale
	TCODConsole::blit(console_, 0, 0, 0, 0, TCODConsole::root, INFOS_POS_X, INFOS_POS_Y);
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
void Gui::renderBar(const int& px, const int& py, const int& pwidth, const std::string& ptext, 
					const float& pvalue, const float& pmaxValue, const TCODColor& pbarColor, 
					const TCODColor& pbackColor)
{
	console_->setDefaultBackground(pbackColor);
	console_->rect(px, py, pwidth, 1, false, TCOD_BKGND_SET);
	
	float widthValue = ( (pvalue * pwidth) / pmaxValue);
	console_->setDefaultBackground(pbarColor);
	console_->rect(px, py, widthValue, 1, false, TCOD_BKGND_SET);
	
	console_->setDefaultForeground(TCODColor::white);
	console_->printEx(px, py, TCOD_BKGND_NONE, TCOD_LEFT, "%s : %i / %i", ptext.c_str(), int(pvalue), int(pmaxValue));
}

/**
 * Ajoute un message à la liste
 * @param color Couleur du texte
 * @param text Texte à afficher
 */
void Gui::message(const TCODColor& pcolor, const std::string ptext, ...)
{
	//construction du texte avec les paramètres
	va_list ap;
	char buff[128];
	va_start(ap, ptext);
	vsprintf(buff, ptext.c_str(), ap);
	va_end(ap);
	
	//la liste est complète, suppression du premier
	if (messages_.size() == MESSAGES_LINES)
	{
		GuiMessage* mess = messages_.get(0);
		messages_.remove(mess);
		delete mess;
	}
	
	messages_.push(new GuiMessage(buff, pcolor));
}

/**
 * Vide la liste des messages
 */
void Gui::clear()
{
	messages_.clearAndDelete();
}

/**
 * Constructeur
 * @param newText Message
 * @param newColor Couleur du texte
 */
GuiMessage::GuiMessage(const std::string& ptext, const TCODColor& pcolor)
{
	text 	= ptext;
	color	= pcolor;
}

/**
 * Destructeur
 */
GuiMessage::~GuiMessage()
{
}