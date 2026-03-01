#ifndef ARMOR_H
#define ARMOR_H
#include "item.h"

class Armor : public Item {
public:
	//constructors
	Armor();
	Armor(string name, string description, float damageReduction);
	
	//getter
	float getDamageReduction();
	
	//setter
	void setDamageReduction(float damageReduction);
private:
	float damageReduction;
};

#endif
