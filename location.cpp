#include "location.h"

//constructors
Location::Location(){
	this->name = "";
}
Location::Location(string name){
	this->name = name;
}

//getters
string Location::getName(){return this->name;}

//setters
void Location::setName(string name){this->name = name;}
