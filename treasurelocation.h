#ifndef TREASURELOCATION_H
#define TREASURELOCATION_H
#include "location.h"
#include "item.h"
#include <vector>

class TreasureLocation : public Location {
public:
	TreasureLocation();
	TreasureLocation(string name, string treasureName);
	
	string getTreasureName();
	vector<Item*>& getRewardItems();
	vector<int>& getRewardChances();
	int getSoulStoneDropChance();     // 0-100 % chance this enemy drops a Soul Stone on death
	int getXpReward();                // flat XP granted when opening the chest
	
	void setTreasureItem(string treasureName);
	void addReward(Item* item, int chance, int count);
	void setSoulStoneDropChance(int chance);
	void setXpReward(int xp);
private:
	string treasureName; //like "golden chest"
	vector<Item*> rewardItems;
	vector<int>   rewardChances;
	int soulStoneDropChance;
	int xpReward;
};

#endif
