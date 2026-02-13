#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include "combatcharacter.h"
using namespace std;

class Enemy : public CombatCharacter{
public:
	//constructors
	Enemy();
	Enemy(string name, float maxHp, float dmg, int level, int difficulty, int speed, int critChance, float critValue, Spell* spells[4]);
	
	//getters
	float getXpToGet();
	int getDifficulty();
	
	//setters
	void setXpToGet(float xpToGet);
	void setDifficulty(int difficulty);
protected:
	float xpToGet;
	int difficulty; //how much does player get per enemy? level*(difficulty*14)
};

#endif
