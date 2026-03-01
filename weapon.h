#ifndef WEAPON_H
#define WEAPON_H
#include "item.h"

class Weapon : public Item {
public:
	//constructor
	Weapon();
	Weapon(string name, string type, string description, float dmg);
	
	//getters
	float getDmg();
	string getType();
	
	//setters
	void setDmg(float dmg);
	void setType(string type);
private:
	float dmg;
	string type;
};

#endif
