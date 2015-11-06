#ifndef stairs_HPP
#define stairs_HPP

#include "entityfixeditem.hpp"
#include "entitymobile.hpp"

class StairUp : public EntityFixedItem
{
public:
    StairUp();
    ~StairUp();

	void render();
	void update();

    void use(EntityMobile* powner);
};

class StairDown : public EntityFixedItem
{
public:
    StairDown();
    ~StairDown();

	void render();
	void update();

    void use(EntityMobile* powner);
};

#endif
