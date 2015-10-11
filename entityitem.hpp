#ifndef EntityItem_HPP
#define EntityItem_HPP

#include "entity.hpp"

class EntityItem : public Entity
{
public:
    EntityItem();
    virtual ~EntityItem();

	void pick();
	void drop();
	virtual void use();
	
private:

};

#endif
