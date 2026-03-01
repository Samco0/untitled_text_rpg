#include "armor.h"

//constructors
Armor::Armor(){
	this->name = "";
	this->description = "";
	this->damageReduction = 0.0;
}
Armor::Armor(string name, string description, float damageReduction, int type){
	this->name = name;
	this->description = description;
	this->damageReduction = damageReduction;
	if(type >= 0 && type <= 5) this->type = type;
}

//getter
float Armor::getDamageReduction(){return this->damageReduction;}
int Armor::getType(){return this->type;}

//setter
void Armor::setDamageReduction(float damageReduction){this->damageReduction = damageReduction;}
void Armor::setType(int type){
	/*
	1 - Helmet
	2 - Chestplate
	3 - Gloves
	4 - Leggings
	5 - Boots
	*/
	
	if(type >= 0 && type <= 5) this->type = type;
}

//output
ostream& operator<<(ostream& output, Armor a){
	output << a.getName() << " | A "; 
	switch (a.getType()) {
	case 1:
		cout << "helmet";
		break;
	case 2:
		cout << "chestplate";
		break;
	case 3:
		cout << "gloves";
		break;
	case 4:
		cout << "leggings";
		break;
	case 5:
		cout << "boots";
		break;
	default:
		break;
	}
	cout << ", " << a.getDamageReduction() << "% dmg reduction, " << a.getDescription();
	return output;
}
