#ifndef COMBATCHARACTER_H
#define COMBATCHARACTER_H
#include "character.h"
#include "chainingspell.h"
#include "statuseffect.h"

//why did i choose to make this class? simple 
//enemy and player have many things in common, 
//but i cant put them in Character due to Character being a parent class to civil characters too (characters who dont fight)

class CombatCharacter : public Character{
public:
	virtual ~CombatCharacter() {}
	
	//constructors
	CombatCharacter();
	CombatCharacter(string name, float maxHp, float currentHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]);
	
	//getters
	float getMaxHp();
	float getCurrentHp();
	float getDmg();
	int getLevel();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	Spell** getSpells();
	StatusEffect** getStatusEffect();
	
	//setters
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setDmg(float dmg);
	void setLevel(int level);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float critValue);
	void setSpells(Spell* spells[4]);
	void setStatusEffect(StatusEffect* statusEffect[4]);
	
	//statuseffect connected functions
	void addStatusEffect(StatusEffect* statusEffect); //this function adds a status at the end, if it doesnt, it means the array is full
	void removeStatusEffect(int arrayIndex); //this function turns a statuseffect into a null pointer, and sorts it to the end of the array
	void checkStatusEffects(); // this function checks if any of the statuses in the array can be turned into a nullptr, if yes, then uses removeStatusEffect
protected:
	float maxHp, currentHp;
	float dmg;
	int level;
	int speed;
	int critChance;
	float critValue;
	Spell* spells[4];
	StatusEffect* statusEffect[4];
	//the combat character can have max 4 status effects, after that he cannot get another one unless one slot frees up
};

#endif

