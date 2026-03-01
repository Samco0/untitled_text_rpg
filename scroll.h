#ifndef SCROLL_H
#define SCROLL_H
#include "item.h"
#include "spell.h"

class Scroll : public Item{
public:
	//constructors
	Scroll();
	Scroll(string description, Spell* heldSpell);
	
	//getters
	Spell* getHeldSpell();
	
	//setters
	void setHeldSpell(Spell* heldSpell);
	
	//output
	friend ostream& operator<<(ostream& output, Scroll s);
private:
	Spell* heldSpell;
};

#endif
