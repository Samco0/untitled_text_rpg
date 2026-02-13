#include "item.h"

//constructors
Item::Item(){
	this->name = "";
	this->description = "";
}
Item::Item(string name, string description){
	this->name = name;
	this->description = description;
}

//getters
string Item::getName(){return this->name;}
string Item::getDescription(){return this->description;}

//setters
void Item::setName(string name){this->name = name;}
void Item::setDescription(string description){this->description = description;}
