#ifndef ARMOR_H
#define ARMOR_H
#include "item.h"

class Armor : public Item {
public:
	//constructors
	Armor();
	Armor(string name, string description, float damageReduction, int type);
	
	//getter
	float getDamageReduction();
	int getType();
	
	//setter
	void setDamageReduction(float damageReduction);
	void setType(int type);
	
	//output
	friend ostream& operator<<(ostream& output, Armor a);
private:
	float damageReduction;
	int type;
};

#endif
