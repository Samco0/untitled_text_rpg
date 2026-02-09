#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include "chainingspell.h"
using namespace std;

class Enemy {
public:
	//constructors
	Enemy();
	Enemy(string name, float maxHp, float dmg, int speed, int critChance, float critValue, Spell* spells[4]);
	
	//getters
	string getName();
	float getMaxHp();
	float getCurrentHp();
	float getDmg();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	Spell** getSpells();
	
	//setters
	void setName(string name);
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setDmg(float dmg);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float getCritValue);
	void setSpells(Spell* spells[4]);
protected:
	string name;
	float maxHp, currentHp;
	float dmg;
	int speed;
	int critChance; // 0-100
	float critValue;
	Spell* spells[4];
};

#endif
