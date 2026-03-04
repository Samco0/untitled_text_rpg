#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"

// Weapon item with damage and type
class Weapon : public Item {
public:
	// Constructors
	Weapon();
	Weapon(string name, string type, string description, float dmg);
	
	// Getters
	float getDmg();
	string getType();
	
	// Setters
	void setDmg(float dmg);
	void setType(string type);
	
	// Output
	friend ostream& operator<<(ostream& output, Weapon w);
	
private:
	float dmg;   // damage dealt by the weapon
	string type; // type of weapon (sword, bow, etc.)
};

#endif
