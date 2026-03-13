#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>
#include "combatcharacter.h"
#include "item.h"
using namespace std;

class Enemy : public CombatCharacter {
public:
	Enemy();
	Enemy(string name, float maxHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]);
	
	// Getters
	float getXpToGet();
	string getDescription();
	vector<Item*>& getRewardItems();
	vector<int>& getRewardChances();
	int getSoulStoneDropChance();     // 0-100 % chance this enemy drops a Soul Stone on death
	
	// Setters
	void setXpToGet(float xpToGet);
	void setDescription(string description);
	void addReward(Item* item, int chance, int count);
	void setSoulStoneDropChance(int chance);
	
	void reset();
	
	void print(ostream& output) override;
	friend ostream& operator<<(ostream& output, Enemy enemy);
	
protected:
	float xpToGet;
	string description;
	vector<Item*> rewardItems;
	vector<int>   rewardChances;
	int soulStoneDropChance;
};

#endif
