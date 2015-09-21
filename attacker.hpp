/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 18/08/2015
 * 
 * L'objet a la capacité à attaquer.
 ******************************************************************************/
 
#ifndef ATTACKER_HPP
#define ATTACKER_HPP

class Attacker
{
public:
    Attacker(Object* powner, const float& ppower);

	float power;
	
	void attack(Object* ptarget);
	
private:
	Object* owner;
};

#endif
