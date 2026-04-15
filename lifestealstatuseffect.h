#ifndef LIFESTEALSTATUSEFFECT_H
#define LIFESTEALSTATUSEFFECT_H
#include "hpstatuseffect.h"

//works similiar to hpstatuseffect, thats why its its parent class
//except this one returns the casters hp to him
class LifestealStatusEffect : public HpStatusEffect {
public:
	//constructors
	LifestealStatusEffect();
	LifestealStatusEffect(string name, int maxDuration, int minDuration, float hpAffection, int lifestealRate);
	
	//getters
	int getLifestealRate();
	
	//setters
	void setLifestealRate(int lifestealRate);
	
	//output
	friend ostream& operator<<(ostream& output, LifestealStatusEffect s);
private:
	int lifestealRate; //0 - 100
};

#endif
