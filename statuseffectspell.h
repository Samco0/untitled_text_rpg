#ifndef STATUSEFFECTSPELL_H
#define STATUSEFFECTSPELL_H

#include "spell.h"
#include "hpstatuseffect.h"
#include "tauntstatuseffect.h"
#include "armorbreakstatuseffect.h"
#include "lifestealstatuseffect.h"

// Spell that can apply a status effect to target
class StatusEffectSpell : public Spell {
public:
	// Constructors
	StatusEffectSpell();
	StatusEffectSpell(string name, string description, float dmg, int fullCooldown, int chanceToRecieve, StatusEffect* statusToGive, string type = "Dark");
	
	// Getters
	int getChanceToRecieve();
	StatusEffect* getStatusToGive();
	
	// Setters
	void setChanceToRecieve(int chanceToRecieve);
	void setStatusToGive(StatusEffect* statusToGive);
	
	// Output
	friend ostream& operator<<(ostream& output, StatusEffectSpell cs);
	
private:
	int chanceToRecieve;        // 1-100 chance to apply the status effect
	StatusEffect* statusToGive; // the status effect this spell gives
};

#endif
