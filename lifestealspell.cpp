#include "lifestealspell.h"

//constructors
LifeStealSpell::LifeStealSpell(){
	this->name = "";
	this->description = "";
	this->dmg = 0;
	this->fullCooldown = 0;
	this->remainingCooldown = 0;
	this->lifeStealRate = 0;
}
LifeStealSpell::LifeStealSpell(string name, string description, float dmg, int fullCooldown, int lifeStealRate, string type){
	this->name = name;
	this->description = description;
	this->dmg = dmg;
	this->fullCooldown = fullCooldown;
	this->remainingCooldown = 0;
	this->lifeStealRate = lifeStealRate;
	this->type = type;
}

//getter
int LifeStealSpell::getLifeStealRate(){return this->lifeStealRate;}

//setter
void LifeStealSpell::setLifeStealRate(int lifeStealRate){this->lifeStealRate = lifeStealRate;}

//output
ostream& operator<<(ostream& output, LifeStealSpell lss){
	output << lss.getName();
	
	if(lss.getRemainingCooldown() == 0) output << endl << " -> Status: Ready" << endl;
	else if(lss.getRemainingCooldown() == 1) output << endl << " -> Status: 1 round until ready" << endl;
	else output << endl << " -> Status: " << lss.getRemainingCooldown() << " rounds until ready" << endl;
	
	cout << " -> Type of spell: Life Steal Spell" << endl;
	output << " -> Element: " << lss.getType() << endl;
	cout << " -> Damage: " << lss.getDmg() << endl;
	cout << " -> Lifesteal rate: " << lss.getLifeStealRate() << "%" << endl;
	output << " -> Description: " << lss.getDescription() << endl << endl;
	return output;
}
