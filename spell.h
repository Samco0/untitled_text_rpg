#ifndef SPELL_H
#define SPELL_H
#include <iostream>

using namespace std;

class Spell {
public:
	virtual ~Spell() {}
	
	//constructors
	Spell();
	Spell(string name, string description, float dmg, int fullCooldown);
	
	//getters
	string getName();
	string getDescription();
	float getDmg();
	int getFullCooldown();
	int getRemainingCooldown();
	
	//setters
	void setName(string name);
	void setDescription(string description);
	void setDmg(float dmg);
	void setFullCooldown(int fullCooldown);
	void setRemainingCooldown(int remainingCooldown);

	//decrease cooldown function
	void decreaseRemainingCooldown();
protected:
	string name;
	string description;
	float dmg;
	int fullCooldown, remainingCooldown;
};

#endif
