/*******************************************************************************
 * RMUSH - RogueLike
 * 
 * Auteur 	: Alexandre JORDAN
 * Date		: 28/08/2015
 *
 * 
 ******************************************************************************/

#ifndef AI_HPP
#define AI_HPP

class Ai
{
public:
	virtual void update() = 0;
	Ai(Object* powner);
	virtual ~Ai() {}
	
protected:
	Object* owner;
};

class MonsterAi : public Ai
{
public:
	MonsterAi(Object* owner);
	void update();
	
protected:
	int moveSpeed;

	void moveOrAttack(const int& ptargetX, const int& ptargetY);
};

class PlayerAi : public Ai
{
public:
	PlayerAi(Object* owner);
	void update();
	Object* choseFromInventary();
	void showInventary();
	void showEquipment();
	
protected:
	bool moveOrAttack(const int& ptargetX, const int& ptargetY);
	void processKey(const int& key);
	bool takeItem(Object* item);
};

#endif