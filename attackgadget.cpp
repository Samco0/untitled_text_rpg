#include "attackgadget.h"

AttackGadget::AttackGadget(){
	this->name = "";
	this->damage = 0.0;
	this->chanceOfBackfire = 0;
	this->type = "";
	this->description = "";
}
AttackGadget::AttackGadget(string name, float damage, int chanceOfBackfire, string type, string description){
	this->name = name;
	this->damage = damage;
	this->chanceOfBackfire = chanceOfBackfire;
	this->type = type;
	this->description = description;
}

float AttackGadget::getDamage(){return this->damage;}
int AttackGadget::getChanceOfBackfire(){return this->chanceOfBackfire;}

void AttackGadget::setDamage(float damage){this->damage = damage;}
void AttackGadget::setChanceOfBackfire(int chanceOfBackfire){this->chanceOfBackfire = chanceOfBackfire;}

ostream& operator<<(ostream& output, AttackGadget gadget){
	output << gadget.getName() << " | A " << gadget.getType() << ", " << gadget.getDamage() << " dmg, " << gadget.getChanceOfBackfire() << "% chance of backfire";
	
	return output;
}
