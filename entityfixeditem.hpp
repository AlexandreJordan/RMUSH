#ifndef EntityFixedItem_HPP
#define EntityFixedItem_HPP

#include "entity.hpp"
#include "entitymobile.hpp"

class EntityFixedItem : public Entity
{
public:
	EntityFixedItem() : Entity() {}
    virtual ~EntityFixedItem() {}

	virtual void use(EntityMobile* powner) = 0;
};

#endif
