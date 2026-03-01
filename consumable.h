#ifndef CONSUMABLE_H
#define CONSUMABLE_H
#include "item.h"

class Consumable : public Item {
public:
	//constructors
	Consumable();
	Consumable(string name, string description, float hpToRecover);
	
	//getters
	float getHpToRecover();
	
	//setters
	void setHpToRecover(float hpToRecover);
	
	//output
	friend ostream& operator<<(ostream& output, Consumable c);
private:
	float hpToRecover;
};

#endif
