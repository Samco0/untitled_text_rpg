#include "combatlocation.h"

//constructors
CombatLocation::CombatLocation(){
	this->name = "";
	this->attacker = nullptr;
}
CombatLocation::CombatLocation(string name, Enemy* attacker){
	this->name = name;
	this->attacker = attacker;
}

//getters
Enemy* CombatLocation::getAttacker(){return this->attacker;}

//setters
void CombatLocation::setAttacker(Enemy* attacker){this->attacker = attacker;}
