#include "armor.h"

//constructors
Armor::Armor(){
	this->name = "";
	this->description = "";
	this->damageReduction = 0.0;
}
Armor::Armor(string name, string description, float damageReduction){
	this->name = name;
	this->description = description;
	this->damageReduction = damageReduction;
}

//getter
float Armor::getDamageReduction(){return this->damageReduction;}

//setter
void Armor::setDamageReduction(float damageReduction){this->damageReduction = damageReduction;}
