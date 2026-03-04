#ifndef SCROLL_H
#define SCROLL_H

#include "item.h"
#include "spell.h"

// Scroll item that holds a spell
class Scroll : public Item {
public:
	// Constructors
	Scroll();
	Scroll(string description, Spell* heldSpell);
	
	// Getter
	Spell* getHeldSpell();
	
	// Setter
	void setHeldSpell(Spell* heldSpell);
	
	// Output
	friend ostream& operator<<(ostream& output, Scroll s);
	
private:
	Spell* heldSpell; // spell stored in this scroll
};

#endif
