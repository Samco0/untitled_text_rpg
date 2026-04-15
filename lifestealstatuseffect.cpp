#include "lifestealstatuseffect.h"

//constructors
LifestealStatusEffect::LifestealStatusEffect(){
	this->name = "";
	this->maxDuration = 0;
	this->minDuration = 0;
	this->currentDuration = 0;
	this->hpAffection = 0;
	this->lifestealRate = 0;
}
LifestealStatusEffect::LifestealStatusEffect(string name, int maxDuration, int minDuration, float hpAffection, int lifestealRate){
	this->name = name;
	this->maxDuration = maxDuration;
	this->minDuration = minDuration;
	this->currentDuration = rand() % (this->maxDuration - this->minDuration + 1) + this->minDuration;
	this->hpAffection = hpAffection;
	this->lifestealRate = lifestealRate;
}

//getters
int LifestealStatusEffect::getLifestealRate(){return this->lifestealRate;}

//setters
void LifestealStatusEffect::setLifestealRate(int lifestealRate){this->lifestealRate = lifestealRate;}

// output
ostream& operator<<(ostream& output, LifestealStatusEffect s){
	output << s.getName() << " (Lifesteal Status Effect) | Health impact: " << s.getHpAffection() << " | Lifesteal rate: " << s.getLifestealRate() << "% | Lasts: " << s.getMinDuration() << " to " << s.getMaxDuration() << " rounds";
	
	return output;
}
