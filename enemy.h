#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include "chainingspell.h"
using namespace std;

class Enemy {
public:
	//constructors
	Enemy();
	Enemy(string name, float maxHp, float maxMana, float dmg, int speed, int critChance, float critValue, Spell* spells[4]);
	
	//getters
	string getName();
	float getMaxHp();
	float getCurrentHp();
	float getMaxMana();
	float getCurrentMana();
	float getDmg();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	Spell** getSpells();
	
	//setters
	void setName(string name);
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setMaxMana(float maxMana);
	void setCurrentMana(float currentMana);
	void setDmg(float dmg);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float getCritValue);
	void setSpells(Spell* spells[4]);
protected:
	string name;
	float maxHp, currentHp;
	float maxMana, currentMana;
	float dmg;
	int speed;
	int critChance; // 0-100
	float critValue;
	Spell* spells[4];
};

#endif
