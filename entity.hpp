#ifndef Entity_HPP
#define Entity_HPP

#include "libtcod.hpp"
#include <string>

class Entity
{
public:
    Entity();
    virtual ~Entity();
	
	virtual void update() = 0;
	virtual void render() = 0;
	
	void setPosition(const int& px, const int& py);		//modifie x et y
	
	int x;												//position de l'entité
	int y;												//...
	
	std::string name;									//nom de l'objet
	std::string description;							//description de l'objet
	
	bool block;											//l'objet bloque le passage
	
	int chr;											//caractère utilisé
	TCODColor color;									//couleur de fond du caractère
};

#endif
