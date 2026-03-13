#ifndef CHAININGSPELL_H
#define CHAININGSPELL_H

#include "spell.h"

// ChainingSpell is a special type of Spell
// that can hit multiple times in one use.
class ChainingSpell : public Spell {
public:
	// Constructors
	ChainingSpell();
	ChainingSpell(string name, string description, float dmg, int fullCooldown, int minHits, int maxHits, string type = "Physical");
	
	// Returns minimum and maximum number of hits
	int getMinHits();
	int getMaxHits();
	
	// Sets hit range
	void setMinHits(int minHits);
	void setMaxHits(int maxHits);
	
	// Enables printing spell info using cout
	friend ostream& operator<<(ostream& output, ChainingSpell cs);
	
private:
	int minHits; // minimum number of hits per cast
	int maxHits; // maximum number of hits per cast
};

#endif
