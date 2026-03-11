#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>
#include "combatcharacter.h"
#include "item.h"
using namespace std;

// Enemy class, inherits combat stuff
class Enemy : public CombatCharacter {
public:
	// Constructors
	Enemy();
	Enemy(string name, float maxHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]);
	
	// Getters
	float getXpToGet();
	vector<Item*>& getRewardItems();
	vector<int>& getRewardChances();
	
	// Setters
	void setXpToGet(float xpToGet);
	void addReward(Item* item, int chance, int count); // % chance 1-100
	
	// Reset enemy stats (lol reset)
	void reset();
	
	// Output
	friend ostream& operator<<(ostream& output, Enemy enemy);
	
protected:
	float xpToGet;                  // XP player gets when defeated
	vector<Item*> rewardItems;      // Items dropped
	vector<int> rewardChances;      // % chances for each item
};

#endif
