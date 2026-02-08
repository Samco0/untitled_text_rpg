#include "spell.h"

//constructors
Spell::Spell(){
	this->name = "";
	this->description = "";
	this->dmg = 0;
	this->fullCooldown = 0;
	this->remainingCooldown = 0;
}
Spell::Spell(string name, string description, float dmg, int fullCooldown){
	this->name = name;
	this->description = description;
	this->dmg = dmg;
	this->fullCooldown = fullCooldown;
	this->remainingCooldown = 0;
}

//getters
string Spell::getName(){return this->name;}
string Spell::getDescription(){return this->description;}
float Spell::getDmg(){return this->dmg;}
int Spell::getFullCooldown(){return this->fullCooldown;}
int Spell::getRemainingCooldown(){return this->remainingCooldown;}

//setters
void Spell::setName(string name){this->name = name;}
void Spell::setDescription(string description){this->description = description;}
void Spell::setDmg(float dmg){this->dmg = dmg;}
void Spell::setFullCooldown(int fullCooldown){this->fullCooldown = fullCooldown;}
void Spell::setRemainingCooldown(int remainingCooldown){this->remainingCooldown = remainingCooldown;}

//decrease cooldown function
void Spell::decreaseRemainingCooldown() {if(this->remainingCooldown != 0) this->remainingCooldown = this->remainingCooldown-1;}
