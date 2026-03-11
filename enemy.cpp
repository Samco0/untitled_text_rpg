#include "enemy.h"

Enemy::Enemy(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = 0;
	this->dmg = 0;
	this->level = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	this->xpToGet = 0;
	this->soulStoneDropChance = 0;
	for(int i=0;i<4;i++) this->spells[i] = nullptr;
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
}

Enemy::Enemy(string name, float maxHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = maxHp;
	this->dmg = dmg;
	this->level = level;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	this->xpToGet = this->level * 10;
	this->soulStoneDropChance = 0;
	for(int i=0;i<4;i++) this->spells[i] = spells[i];
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
}

float Enemy::getXpToGet(){ return this->xpToGet; }
vector<Item*>& Enemy::getRewardItems(){ return this->rewardItems; }
vector<int>& Enemy::getRewardChances(){ return this->rewardChances; }
int Enemy::getSoulStoneDropChance(){ return this->soulStoneDropChance; }

void Enemy::setXpToGet(float xpToGet){ this->xpToGet = xpToGet; }
void Enemy::setSoulStoneDropChance(int chance){
	if (chance < 0) chance = 0;
	if (chance > 100) chance = 100;
	this->soulStoneDropChance = chance;
}

void Enemy::addReward(Item* item, int chance, int count){
	for(int i = 0; i < count; i++){
		rewardItems.push_back(item);
		rewardChances.push_back(chance);
	}
}

void Enemy::reset(){
	this->setCurrentHp(this->getMaxHp());
}

ostream& operator<<(ostream& output, Enemy enemy){
	output << enemy.getName() << endl;
	output << " -> Vitality: " << enemy.getCurrentHp() << "/" << enemy.getMaxHp() << " hp" << endl;
	output << " -> Wisdom: " << enemy.getLevel() << ". level" << endl;
	output << " -> Agility: " << enemy.getSpeed() << endl;
	return output;
}
