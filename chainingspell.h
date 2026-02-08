#ifndef CHAININGSPELL_H
#define CHAININGSPELL_H
#include "spell.h"

class ChainingSpell : public Spell{
public:
	//constructors
	ChainingSpell();
	ChainingSpell(string name, string description, float dmg, int fullCooldown, int minHits, int maxHits);
	
	//getters
	int getMinHits();
	int getMaxHits();
	
	//setters
	void setMinHits(int minHits);
	void setMaxHits(int maxHits);
private:
	int minHits;
	int maxHits;
};

#endif
