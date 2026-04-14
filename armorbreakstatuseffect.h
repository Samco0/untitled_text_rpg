#ifndef ARMORBREAKSTATUSEFFECT_H
#define ARMORBREAKSTATUSEFFECT_H

#include "statuseffect.h"

// Status effect type 1: disables the player's armor for a few rounds
// Armor pieces are still equipped, but their damage reduction is ignored
// Does not deal any damage - purely a debuff
class ArmorBreakStatusEffect : public StatusEffect {
public:
	// Constructors
	ArmorBreakStatusEffect();
	ArmorBreakStatusEffect(string name, int maxDuration, int minDuration);
	
	//output
	friend ostream& operator<<(ostream& output, ArmorBreakStatusEffect s);
};

#endif
