#ifndef LIFESTEALSPELL_H
#define LIFESTEALSPELL_H
#include "spell.h"

class LifeStealSpell : public Spell{
public:
	//constructors
	LifeStealSpell();
	LifeStealSpell(string name, string description, float dmg, int fullCooldown, int lifeStealRate);
	
	//getter
	int getLifeStealRate();
	
	//setter
	void setLifeStealRate(int lifeStealRate);
	
	//output
	friend ostream& operator<<(ostream& output, LifeStealSpell lss);
private:
	int lifeStealRate;
};

#endif
