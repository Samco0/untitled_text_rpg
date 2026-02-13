#include "hpstatuseffect.h"

//constructors
HpStatusEffect::HpStatusEffect(){
	this->name = "";
	this->maxDuration = 0;
	this->minDuration = 0;
	this->currentDuration = 0;
	this->hpAffection = 0;
}
HpStatusEffect::HpStatusEffect(string name, int maxDuration, int minDuration, float hpAffection){
	this->name = name;
	this->maxDuration = maxDuration;
	this->minDuration = minDuration;
	this->currentDuration = rand() % (this->maxDuration - this->minDuration + 1) + this->minDuration;
	this->hpAffection = hpAffection;
}

//getters
float HpStatusEffect::getHpAffection(){return this->hpAffection;}

//setters
void HpStatusEffect::setHpAffection(float hpAffection){this->hpAffection = hpAffection;}
