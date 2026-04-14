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
StatusEffectSpell::StatusEffectSpell(string name, string description, float dmg, int fullCooldown, int chanceToRecieve, StatusEffect* statusToGive, string type){
	this->name = name;
	this->description = description;
	this->dmg = dmg;
	this->fullCooldown = fullCooldown;
	this->remainingCooldown = 0;
	this->chanceToRecieve = chanceToRecieve;
	this->statusToGive = statusToGive;
	this->type = type;
}

//getters
int StatusEffectSpell::getChanceToRecieve(){return this->chanceToRecieve;}
StatusEffect* StatusEffectSpell::getStatusToGive(){return this->statusToGive;}

//setter
void StatusEffectSpell::setChanceToRecieve(int chanceToRecieve){this->chanceToRecieve = chanceToRecieve;}
void StatusEffectSpell::setStatusToGive(StatusEffect* statusToGive){this->statusToGive = statusToGive;}

ostream& operator<<(ostream& output, StatusEffectSpell sas) {
	output << sas.getName() << endl;
	
	if (sas.getRemainingCooldown() == 0)      output << " -> Status: Ready" << endl;
	else if (sas.getRemainingCooldown() == 1) output << " -> Status: 1 round until ready" << endl;
	else                                       output << " -> Status: " << sas.getRemainingCooldown() << " rounds until ready" << endl;
	
	output << " -> Element: " << sas.getType() << endl;
	output << " -> Damage: " << sas.getDmg() << endl;
	
	// správně přes pointer, ne dereference
	StatusEffect* se = sas.getStatusToGive();
	HpStatusEffect* hps = dynamic_cast<HpStatusEffect*>(se);
	TauntStatusEffect* ts = dynamic_cast<TauntStatusEffect*>(se);
	if (hps != nullptr) output << " -> Effect: " << *hps << endl;
	else if (se != nullptr) output << " -> Effect: " << *se << endl;
	else if (ts != nullptr) output << " -> Effect: " << *ts << endl;
	
	output << " -> Chance: " << sas.getChanceToRecieve() << "%" << endl;
	output << " -> " << sas.getDescription() << endl << endl;
	return output;
}
