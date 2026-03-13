#ifndef COMBATCHARACTER_H
#define COMBATCHARACTER_H

#include "character.h"
#include "chainingspell.h"
#include "statuseffect.h"

// This class is for characters that fight (player or enemy)
// Can't put fighting stuff in Character, because some characters don't fight

class CombatCharacter : public Character {
public:
	virtual ~CombatCharacter() {}
	
	// Constructors
	CombatCharacter();
	CombatCharacter(string name, float maxHp, float currentHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]);
	
	// Getters
	float getMaxHp();
	float getCurrentHp();
	float getDmg();
	int getLevel();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	Spell** getSpells();
	StatusEffect** getStatusEffect();
	
	// Setters
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setDmg(float dmg);
	void setLevel(int level);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float critValue);
	void setSpells(Spell* spells[4]);
	void setSpell(int index, Spell* spell);
	void setStatusEffect(StatusEffect* statusEffect[4]);
	
	// Status effect stuff
	void addStatusEffect(StatusEffect* statusEffect);   // add to end, if full, do nothing
	void removeStatusEffect(int arrayIndex);             // remove and push nullptr to end
	void checkStatusEffects();                           // check if any can be removed
	
	// Output
	virtual void print(ostream& output);
	friend ostream& operator<<(ostream& output, CombatCharacter& character);
	
protected:
	float maxHp, currentHp;
	float dmg;
	int level;
	int speed;
	int critChance;
	float critValue;
	Spell* spells[4];
	StatusEffect* statusEffect[4];  // max 4, can't add more unless a slot frees
};

#endif
