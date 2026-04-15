#include "heallocation.h"


HealLocation::HealLocation(){
	this->name = "";
	this->healName = "";
}
HealLocation::HealLocation(string name, string healName){
	this->name = name;
	this->healName = healName;
}

string HealLocation::getHealName(){ return this->healName; }

void HealLocation::setHealName(string healName){ this->healName = healName; }
