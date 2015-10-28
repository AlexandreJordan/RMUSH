#ifndef EntityItem_HPP
#define EntityItem_HPP

#include "entity.hpp"
#include "entitymobile.hpp"

class EntityItem : public Entity
{
public:
	EntityItem() : Entity(), isSelected(false) {}
    virtual ~EntityItem() {}

	virtual void use(EntityMobile* powner) = 0;
	
	bool isSelected;
};

#endif
