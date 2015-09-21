#ifndef Container_HPP
#define Container_HPP

class Container
{
public:
    Container(const int& pmaxSize);
    ~Container();

	int getSize() { return inventory.size(); }

	bool add(Object* pobject);
	void remove(Object* pobject);
	
	TCODList<Object*>& getInventory() { return inventory; }
private:
	TCODList<Object*> inventory;

	int sizeMax;
};

#endif
