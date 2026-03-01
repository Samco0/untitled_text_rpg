#include "chainingspell.h"

//constructors
ChainingSpell::ChainingSpell(){
	this->name = "";
	this->description = "";
	this->dmg = 0;
	this->fullCooldown = 0;
	this->remainingCooldown = 0;
	this->minHits = 0;
	this->maxHits = 0;
}
ChainingSpell::ChainingSpell(string name, string description, float dmg, int fullCooldown, int minHits, int maxHits){
	this->name = name;
	this->description = description;
	this->dmg = dmg;
	this->fullCooldown = fullCooldown;
	this->remainingCooldown = 0;
	this->minHits = minHits;
	this->maxHits = maxHits;
}

//getters
int ChainingSpell::getMinHits(){return this->minHits;}
int ChainingSpell::getMaxHits(){return this->maxHits;}

//setters
void ChainingSpell::setMinHits(int minHits){this->minHits = minHits;}
void ChainingSpell::setMaxHits(int maxHits){this->maxHits = maxHits;}

//output
ostream& operator<<(ostream& output, ChainingSpell cs){
	output << cs.getName();
	
	if(cs.getRemainingCooldown() == 0) output << endl << " -> Status: Ready" << endl;
	else if(cs.getRemainingCooldown() == 1) output << endl << " -> Status: 1 round until ready" << endl;
	else output << endl << " -> Status: " << cs.getRemainingCooldown() << " rounds until ready" << endl;
	
	output << " -> Type of spell: Chaining attack spell" << endl;
	output << " -> Damage per hit: " << cs.getDmg() << " damage" << endl;
	output << " -> Minimal hits: " << cs.getMinHits() << " hits" << endl;
	output << " -> Maximal hits: " << cs.getMaxHits() << " hits" << endl;
	
	output << " -> Description: " << cs.getDescription() << endl << endl;
	return output;
}
