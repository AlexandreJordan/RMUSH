#include "object.hpp"
#include "engine.hpp"
#include <math.h>

using namespace std;

/**
 * Constructeur
 * @param x Position de l'objet
 * @param y Position de l'objet
 * @param pchr Caractère utilisé pour l'affichage
 * @param pname Nom de l'objet
 * @param pblock L'objet est bloquant pour les autres objets
 * @param pcol Couleur de l'objet
 */
Object::Object(const int &px, const int &py, const int& pchr, const char* pname, const char* pdesc, const bool& pblock, const TCODColor& pcol)
{
	posX 			= px;
	posY 			= py;
    chr 			= pchr;
    color 			= pcol;
	name			= pname;
	description		= pdesc;
	block			= pblock;
	destructible 	= NULL;
	ai				= NULL;
	attacker		= NULL;
	container		= NULL;
	usable			= NULL;
}

/**
 * Destructeur 
 */
Object::~Object()
{
	if (destructible) 	delete destructible;
	if (ai) 			delete ai;
	if (attacker) 		delete attacker;
	if (container) 		delete container;
	if (usable) 		delete usable;
}

/**
 * Rendu de l'object
 */
void Object::render() const
{
    TCODConsole::root->setChar(posX, posY, chr);
	TCODConsole::root->setCharForeground(posX, posY, color);
}

/**
 * Mise à jour de l'objet
 * 	- Mise à jour de son AI
 */
void Object::update()
{
	if (ai) ai->update();
}

/**
 * Vérifie si l'objet est à la position px, py
 * @param px Position donnée à vérifier
 * @param py Position donnée à vérifier
 */
bool Object::isInPosition(const int& px, const int& py)
{
	return (posX == px && posY == py);
}

/**
 * Donne la distance entre l'objet et la position donnée
 * @param px Position donnée
 * @param py Position donnée
 */
float Object::getDistance(const int& px, const int& py)
{
	int deltaX = this->posX - px;
	int deltaY = this->posY - py;
	
	return sqrtf( (deltaX * deltaX) + (deltaY * deltaY) );
}
