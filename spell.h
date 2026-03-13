#ifndef SPELL_H
#define SPELL_H

#include <iostream>
using namespace std;

// Base class for all spells
class Spell {
public:
	virtual ~Spell() {}
	
	// Constructors
	Spell();
	Spell(string name, string description, float dmg, int fullCooldown, string type = "Physical");
	
	// Getters
	string getName();
	string getDescription();
	float getDmg();
	int getFullCooldown();
	int getRemainingCooldown();
	string getType();
	
	// Setters
	void setName(string name);
	void setDescription(string description);
	void setDmg(float dmg);
	void setFullCooldown(int fullCooldown);
	void setRemainingCooldown(int remainingCooldown);
	void setType(string type);
	
	// Decrease cooldown by 1 (can't go below 0)
	void decreaseRemainingCooldown();
	
	// Output
	friend ostream& operator<<(ostream& output, Spell s);
	
protected:
	string name;            // spell name
	string description;     // what it does
	float dmg;              // damage dealt
	int fullCooldown;       // max cooldown
	int remainingCooldown;  // current cooldown
	string type;            // spell type (e.g. Fire, Blood, Water, Physical)
};

#endif
