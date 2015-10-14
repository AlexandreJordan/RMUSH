#ifndef EntityItem_HPP
#define EntityItem_HPP

#include "entity.hpp"
#include "entitymobile.hpp"

class EntityItem : public Entity
{
public:
	EntityItem() : Entity() {}
    virtual ~EntityItem() {}

	virtual void use(EntityMobile* powner, EntityMobile* ptarget) = 0;
};

#endif
