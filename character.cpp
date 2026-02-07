#include "character.h"

//constructor
Character::Character(){
	this->name = "";
}
Character::Character(string name){
	this->name = name;
}

//getters
string Character::getName(){return this->name;}

//setters
void Character::setName(string name){this->name = name;}
