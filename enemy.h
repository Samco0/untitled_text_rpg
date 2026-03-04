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
	
	// getters
	float getXpToGet();
	vector<Item*>& getRewardItems();
	vector<int>& getRewardChances();
	
	// setters
	void setXpToGet(float xpToGet);
	void addReward(Item* item, int chance); // chance in % 1-100
	
	// resetter (hahahah get it, it resets ahahahahahhaha)
	void reset();
	
	// output
	friend ostream& operator<<(ostream& output, Enemy player);
protected:
	float xpToGet;
	vector<Item*> rewardItems;
	vector<int> rewardChances;
};

#endif
