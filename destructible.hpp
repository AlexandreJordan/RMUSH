/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 18/08/2015
 * 
 * L'objet est destructible / mortel
 ******************************************************************************/
 
#ifndef DESTRUCTIBLE_HPP
#define DESTRUCTIBLE_HPP

class Destructible
{
public:
    Destructible(Object* powner, const float& php, const float& pmaxHp);
	virtual ~Destructible() {}
	
	float maxHp;
	float hp;
	
	bool isDead() { return hp <= 0; }
	float takeDamage(const float& pdamage);
	float heal(const float& plife);
	virtual void die();
	
protected:
	Object* owner;
};


class MonsterDestructible : public Destructible
{
public:
	MonsterDestructible(Object* powner, const float& php, const float& pmaxHp);
	void die();
};


class PlayerDestructible : public Destructible
{
public:
	PlayerDestructible(Object* powner, const float& php, const float& pmaxHp);
	void die();
};

#endif