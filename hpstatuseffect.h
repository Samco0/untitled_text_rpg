#ifndef HPSTATUSEFFECT_H
#define HPSTATUSEFFECT_H
#include "statuseffect.h"

class HpStatusEffect : public StatusEffect{
public:
	//constructors
	HpStatusEffect();
	HpStatusEffect(string name, int maxDuration, int minDuration, float hpAffection);
	
	//getters
	float getHpAffection();
	
	//setters
	void setHpAffection(float hpAffection);
private:
	float hpAffection; //affection as "what it does" to hp, can be both + and -
};

#endif
