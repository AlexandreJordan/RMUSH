#ifndef Pickable_HPP
#define Pickable_HPP

/******************************************************************************
 *                                    USABLE                                  *
 ******************************************************************************/
class Usable
{
public:
    Usable(Object* owner);
    virtual ~Usable() {}

	bool pick(Object* pwearer);
	void drop(Object* pwearer);
	virtual bool use(Object* pwearer);

protected:
	Object* owner;
};


/******************************************************************************
 *                                 HEALTHPOTION                               *
 ******************************************************************************/
class HealthPotion : public Usable
{
public:
	HealthPotion(Object* owner, const int& plife);
	
	bool use(Object* pwearer);

private:
	int life;
};


/******************************************************************************
 *                                   CROSSBOW                                 *
 ******************************************************************************/
class CrossBow : public Usable
{
public :
	CrossBow(Object* powner, const float& prange, const float& pdamage);
	
	bool use(Object* pwearer);

private:
	float range;
	float damage;
};
 
#endif
