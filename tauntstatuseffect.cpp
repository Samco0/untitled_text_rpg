#include "tauntstatuseffect.h"

// Default constructor
TauntStatusEffect::TauntStatusEffect() {
	this->name = "";
	this->maxDuration = 0;
	this->minDuration = 0;
	this->currentDuration = 0;
	this->currentRound = 0;
}

// Main constructor - duration is randomised between min and max
TauntStatusEffect::TauntStatusEffect(string name, int maxDuration, int minDuration) {
	this->name = name;
	this->maxDuration = maxDuration;
	this->minDuration = minDuration;
	this->currentDuration = rand() % (this->maxDuration - this->minDuration + 1) + this->minDuration;
	this->currentRound = 0;
}

//output
ostream& operator<<(ostream& output, TauntStatusEffect s){
	output << s.getName() << " (Taunt Status Effect) | Lasts: " << s.getMinDuration() << " to " << s.getMaxDuration() << " rounds";
	
	return output;
}
