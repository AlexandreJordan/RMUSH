/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 22/08/2015
 * 
 * Gestion du joueur
 ******************************************************************************/
 
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include "libtcod.hpp"
#include "main.hpp"

class Object
{
public:
	Object(const int &px, const int &py, const int& pchr, const char* pname, const char* pdesc, const bool& pblock, const TCODColor& pcol);
	~Object();
	
	void render() const;
	void update();

	bool isInPosition(const int& px, const int& py);						//vérifie si la position de l'objet est x,y
	float getDistance(const int& px, const int& py);						//retourne la disance entre this et x,y

	int posX; 																//position de l'animé
	int posY;																//...
	
	const char* name;														//nom de l'objet
	const char* description;												//description de l'objet
	
	bool block;																//l'objet bloque le passage
	
	Destructible* destructible;												//propriétés de l'objet
	Ai* ai;																	//...
	Attacker* attacker;														//...
	Container* container;													//...
	Usable* usable;															//...
	
	int chr;																//caractère utilisé
	TCODColor color;														//couleur de fond du caractère
};

#endif