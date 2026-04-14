#include "multiplecombatlocation.h"

MultipleCombatLocation::MultipleCombatLocation(){}
MultipleCombatLocation::MultipleCombatLocation(string name, vector<CombatLocation*> locations){
	this->name = name;
	this->locations = locations;
}

vector<CombatLocation*>& MultipleCombatLocation::getLocations() {return this->locations;}
bool MultipleCombatLocation::getIsEmpty() {return this->isEmpty;}

void MultipleCombatLocation::setIsEmpty(bool isEmpty) {this->isEmpty = isEmpty;}

void MultipleCombatLocation::addLocation(CombatLocation* location){if(location) this->locations.push_back(location);}
void MultipleCombatLocation::deleteLocation(int index){
	if(index < 0 || index >= (int)this->locations.size()) return;
	this->locations.erase(this->locations.begin() + index);
}
