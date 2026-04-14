#ifndef HPSTATUSEFFECT_H
#define HPSTATUSEFFECT_H

#include "statuseffect.h"

// Status effect that changes HP (can heal or damage)
class HpStatusEffect : public StatusEffect {
public:
	// Constructors
	HpStatusEffect();
	HpStatusEffect(string name, int maxDuration, int minDuration, float hpAffection);
	
	// Getter
	float getHpAffection();
	
	// Setter
	void setHpAffection(float hpAffection);
	
	//output
	friend ostream& operator<<(ostream& output, HpStatusEffect s);
private:
	float hpAffection; // how it affects HP, can be + or -
};

#endif
