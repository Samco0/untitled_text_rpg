#include "treasurelocation.h"

TreasureLocation::TreasureLocation(){
	this->name = "";
	this->treasureName = "";
	this->xpReward = 0;
}
TreasureLocation::TreasureLocation(string name, string treasureName){
	this->name = name;
	this->treasureName = treasureName;
	this->xpReward = 0;
}

string TreasureLocation::getTreasureName(){return this->treasureName;}
vector<Item*>& TreasureLocation::getRewardItems(){return this->rewardItems;}
vector<int>& TreasureLocation::getRewardChances(){return this->rewardChances;}
int TreasureLocation::getSoulStoneDropChance(){return this->soulStoneDropChance;}
int TreasureLocation::getXpReward(){return this->xpReward;}

void TreasureLocation::setTreasureItem(string treasureName){this->treasureName = treasureName;}
void TreasureLocation::addReward(Item* item, int chance, int count){
	for(int i = 0; i < count; i++){
		rewardItems.push_back(item);
		rewardChances.push_back(chance);
	}
}
void TreasureLocation::setSoulStoneDropChance(int chance){this->soulStoneDropChance = chance;}
void TreasureLocation::setXpReward(int xp){this->xpReward = xp < 0 ? 0 : xp;}
