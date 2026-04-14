#include "gadget.h"

Gadget::Gadget(){
	this->name = "";
	this->description = "";
}
Gadget::Gadget(string name, string descriptions){
	this->name = name;
	this->description = description;
}

string Gadget::getType(){return this->type;}

void Gadget::setType(string type){this->type = type;}
