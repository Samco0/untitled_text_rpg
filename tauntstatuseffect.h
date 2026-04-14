#ifndef TAUNTSTATUSEFFECT_H
#define TAUNTSTATUSEFFECT_H

#include "statuseffect.h"

// Status effect type 2: the holder cannot use spells
// If the player tries to cast a spell, they are forced into a normal attack instead
// Does not deal damage - purely a debuff
class TauntStatusEffect : public StatusEffect {
public:
	// Constructors
	TauntStatusEffect();
	TauntStatusEffect(string name, int maxDuration, int minDuration);
	
	//output
	friend ostream& operator<<(ostream& output, TauntStatusEffect s);
};

#endif
