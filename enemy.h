#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>
#include <vector>
#include "combatcharacter.h"
#include "item.h"
using namespace std;

class Enemy : public CombatCharacter{
public:
	Enemy();
	Enemy(string name, float maxHp, float dmg, int level, int difficulty, int speed, int critChance, float critValue, Spell* spells[4]);
	
	// getters
	float getXpToGet();
	int getDifficulty();
	vector<Item*>& getRewardItems() { return rewardItems; }
	vector<int>& getRewardChances() { return rewardChances; }
	
	// setters
	void setXpToGet(float xpToGet);
	void setDifficulty(int difficulty);
	void addReward(Item* item, int chance); // chance in % 1-100
	
	// output
	friend ostream& operator<<(ostream& output, Enemy player);
protected:
	float xpToGet;
	int difficulty;
	vector<Item*> rewardItems;
	vector<int> rewardChances;
};

#endif
