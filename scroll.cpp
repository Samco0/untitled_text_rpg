#include "scroll.h"

//constructors
Scroll::Scroll(){
	this->name = "";
	this->description = "";
	this->heldSpell = nullptr;
}
Scroll::Scroll(string description, Spell* heldSpell){
	this->name = "Scroll of " + heldSpell->getName();
	this->description = description;
	this->heldSpell = heldSpell;
}

//getters
Spell* Scroll::getHeldSpell(){return heldSpell;}

//setters
void Scroll::setHeldSpell(Spell* heldSpell){this->heldSpell = heldSpell;}

//output
ostream& operator<<(ostream& output, Scroll s){
	output << s.getName() << " | Contains " << s.getHeldSpell()->getName() << ", " << s.getDescription();
	
	return output;
}
