#ifndef STATUSEFFECTSPELL_H
#define STATUSEFFECTSPELL_H
#include "spell.h"
#include "hpstatuseffect.h"

class StatusEffectSpell : public Spell{
public:
	//constructor
	StatusEffectSpell();
	StatusEffectSpell(string name, string description, float dmg, int fullCooldown, int chanceToRecieve, StatusEffect* statusToGive);
	
	//getters
	int getChanceToRecieve();
	StatusEffect* getStatusToGive();
	
	//setter
	void setChanceToRecieve(int chanceToRecieve);
	void setStatusToGive(StatusEffect* statusToGive);
	
	//output
	friend ostream& operator<<(ostream& output, StatusEffectSpell cs);
private:
	int chanceToRecieve; //1-100 chance to recieve the status effect, that is held by this spell
	StatusEffect* statusToGive; //status effect held by this spell
};

#endif
