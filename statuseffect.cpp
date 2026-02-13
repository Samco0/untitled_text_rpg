#include "statuseffect.h"

//constructor
StatusEffect::StatusEffect(){
	this->name = "";
	this->maxDuration = 0;
	this->minDuration = 0;
	this->currentDuration = 0;
	this->currentRound = 0;
}
StatusEffect::StatusEffect(string name, int maxDuration, int minDuration){
	this->name = name;
	this->maxDuration = maxDuration;
	this->minDuration = minDuration;
	this->currentDuration = rand() % (this->maxDuration - this->minDuration + 1) + this->minDuration;
	this->currentRound = 0;
}

//getters
string StatusEffect::getName(){return this->name;}
int StatusEffect::getMaxDuration(){return this->maxDuration;}
int StatusEffect::getMinDuration(){return this->minDuration;}
int StatusEffect::getCurrentDuration(){return this->currentDuration;}
int StatusEffect::getCurrentRound(){return this->currentRound;}

//setters
void StatusEffect::setName(string name){this->name = name;}
void StatusEffect::setMaxDuration(int maxDuration){this->maxDuration = maxDuration;}
void StatusEffect::setMinDuration(int minDuration){this->minDuration = minDuration;}
void StatusEffect::setCurrentDuration(int currentDuration){this->currentDuration = currentDuration;}
void StatusEffect::setCurrentRound(int currentRound){this->currentRound = currentRound;}

//function to increase currentRound by one
void StatusEffect::increaseRound(){
	this->currentRound += 1;
}
