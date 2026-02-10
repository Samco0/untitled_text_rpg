#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"
#include "chainingspell.h"

class Player : public Character{
public:
	//constructor
	Player();
	Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Spell* spells[4]);
	
	//getters
	float getMaxHp();
	float getCurrentHp();
	float getDmg();
	float getRequireXp();
	float getCurrentXp();
	int getLevel();
	int getBalance();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	Spell** getSpells();
	
	//setters
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setDmg(float dmg);
	void setRequireXp(float requireXp);
	void setCurrentXp(float currentXp);
	void setLevel(int level);
	void setBalance(int balance);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float getCritValue);
	void setSpells(Spell* spells[4]);
protected:
	float maxHp, currentHp;
	float dmg;
	float requireXp, currentXp;
	int level;
	/*
	from level 1 to level 2 it requires 15xp
	for each next level it requires the previous amount x1.4
	*/
	int balance;
	int speed;
	int critChance; //0-100
	float critValue;
	Spell* spells[4];
};

#endif
