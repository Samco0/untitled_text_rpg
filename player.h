#ifndef PLAYER_H
#define PLAYER_H
#include "character.h"

class Player : public Character{
public:
	//constructor
	Player();
	Player(string name, float maxHp, float maxMana, float dmg, int balance, int speed, int critChance, float critValue);
	
	//getters
	float getMaxHp();
	float getCurrentHp();
	float getMaxMana();
	float getCurrentMana();
	float getDmg();
	int getBalance();
	int getSpeed();
	int getCritChance();
	float getCritValue();
	
	//setters
	void setMaxHp(float maxHp);
	void setCurrentHp(float currentHp);
	void setMaxMana(float maxMana);
	void setCurrentMana(float currentMana);
	void setDmg(float dmg);
	void setBalance(int balance);
	void setSpeed(int speed);
	void setCritChance(int critChance);
	void setCritValue(float getCritValue);
protected:
	float maxHp, currentHp;
	float maxMana, currentMana;
	float dmg;
	int balance;
	int speed;
	int critChance; //0-100
	float critValue;
};

#endif
