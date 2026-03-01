#include "weapon.h"

//constructor
Weapon::Weapon(){
	this->name = "";
	this->type = "";
	this->description = "";
	this->dmg = 0;
}
Weapon::Weapon(string name, string type, string description, float dmg){
	this->name = name;
	this->type = type;
	this->description = description;
	this->dmg = dmg;
}

//getters
float Weapon::getDmg(){return this->dmg;}
string Weapon::getType(){return this->type;}

//setters
void Weapon::setDmg(float dmg){this->dmg = dmg;}
void Weapon::setType(string type){this->type = type;}
