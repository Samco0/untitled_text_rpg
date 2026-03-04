#include "armor.h"

// constructors
Armor::Armor(){
	this->name = "";                // set default name to empty
	this->description = "";         // set default description to empty
	this->damageReduction = 0.0;   // default no damage reduction
}
Armor::Armor(string name, string description, float damageReduction, int type){
	this->name = name;              // set name from parameter
	this->description = description;// set description from parameter
	this->damageReduction = damageReduction; // set damage reduction
	if(type >= 0 && type <= 5) this->type = type; // only accept valid type 0-5
}

// getter
float Armor::getDamageReduction(){return this->damageReduction;} // returns the percentage of damage reduction
int Armor::getType(){return this->type;} // returns the armor type as int

// setter
void Armor::setDamageReduction(float damageReduction){this->damageReduction = damageReduction;} // set damage reduction value
void Armor::setType(int type){
	/*
	1 - Helmet
	2 - Chestplate
	3 - Gloves
	4 - Leggings
	5 - Boots
	*/
	
	if(type >= 0 && type <= 5) this->type = type; // only assign valid type
}

// output
ostream& operator<<(ostream& output, Armor a){
	output << a.getName() << " | A "; // print armor name and "A" as prefix
	switch (a.getType()) {
	case 1:
		cout << "helmet"; // type 1 = helmet
		break;
	case 2:
		cout << "chestplate"; // type 2 = chestplate
		break;
	case 3:
		cout << "gloves"; // type 3 = gloves
		break;
	case 4:
		cout << "leggings"; // type 4 = leggings
		break;
	case 5:
		cout << "boots"; // type 5 = boots
		break;
	default:
		break; // do nothing if type invalid
	}
	cout << ", " << a.getDamageReduction() << "% dmg reduction, " << a.getDescription(); // print damage reduction and description
	return output; // return the ostream
}
