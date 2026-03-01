#include "statuseffectspell.h"

//constructor
StatusEffectSpell::StatusEffectSpell(){
	this->name = "";
	this->description = "";
	this->dmg = 0;
	this->fullCooldown = 0;
	this->remainingCooldown = 0;
	this->chanceToRecieve = 0;
	this->statusToGive = nullptr;
}
StatusEffectSpell::StatusEffectSpell(string name, string description, float dmg, int fullCooldown, int chanceToRecieve, StatusEffect* statusToGive){
	this->name = name;
	this->description = description;
	this->dmg = dmg;
	this->fullCooldown = fullCooldown;
	this->remainingCooldown = remainingCooldown;
	this->chanceToRecieve = chanceToRecieve;
	this->statusToGive = statusToGive;
}

//getters
int StatusEffectSpell::getChanceToRecieve(){return this->chanceToRecieve;}
StatusEffect* StatusEffectSpell::getStatusToGive(){return this->statusToGive;}

//setter
void StatusEffectSpell::setChanceToRecieve(int chanceToRecieve){this->chanceToRecieve = chanceToRecieve;}
void StatusEffectSpell::setStatusToGive(StatusEffect* statusToGive){this->statusToGive = statusToGive;}

//output
ostream& operator<<(ostream& output, StatusEffectSpell sas){
	output << sas.getName();
	
	if(sas.getRemainingCooldown() == 0) output << endl << " -> Status: Ready" << endl;
	else if(sas.getRemainingCooldown() == 1) output << endl << " -> Status: 1 round until ready" << endl;
	else output << endl << " -> Status: " << sas.getRemainingCooldown() << " rounds until ready" << endl;
	
	output << " -> Type of spell: Status effect spell" << endl;
	output << " -> Damage: " << sas.getDmg() << endl;
	output << " -> Status effect: " << sas.getStatusToGive()->getName() << endl;
	
	output << " -> Description: " << sas.getDescription() << endl << endl;
	return output;
}
