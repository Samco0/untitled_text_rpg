#ifndef LIFESTEALSPELL_H
#define LIFESTEALSPELL_H

#include "spell.h"

// Spell that damages and heals caster a bit (lifesteal)
class LifeStealSpell : public Spell {
public:
	// Constructors
	LifeStealSpell();
	LifeStealSpell(string name, string description, float dmg, int fullCooldown, int lifeStealRate);
	
	// Getter
	int getLifeStealRate();
	
	// Setter
	void setLifeStealRate(int lifeStealRate);
	
	// Output
	friend ostream& operator<<(ostream& output, LifeStealSpell lss);
	
private:
	int lifeStealRate; // % of damage returned as HP
};

#endif
