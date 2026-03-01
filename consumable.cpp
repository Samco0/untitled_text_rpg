#include "consumable.h"

//constructors
Consumable::Consumable(){
	this->name = "";
	this->description = "";
	this->hpToRecover = 0.0;
}
Consumable::Consumable(string name, string description, float hpToRecover){
	this->name = name;
	this->description = description;
	this->hpToRecover = hpToRecover;
}

//getters
float Consumable::getHpToRecover(){return this->hpToRecover;}

//setters
void Consumable::setHpToRecover(float hpToRecover){this->hpToRecover = hpToRecover;}

//output
ostream& operator<<(ostream& output, Consumable c){
	output << c.getName() << " | Recovers " << c.getHpToRecover() << " hp, " << c.getDescription();
	
	return output;
}
