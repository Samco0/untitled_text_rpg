#include "enemy.h"

// constructors
Enemy::Enemy(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = this->maxHp;
	this->dmg = 0;
	this->level = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	this->xpToGet = 0;
	
	for(int i=0;i<4;i++) this->spells[i] = nullptr;
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
}

Enemy::Enemy(string name, float maxHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = this->maxHp;
	this->dmg = dmg;
	this->level = level;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	
	this->xpToGet = this->level * 10; // jednoduchá formule, můžeš upravit
	
	for(int i=0;i<4;i++) this->spells[i] = spells[i];
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
}

// getters
float Enemy::getXpToGet(){ return this->xpToGet; }
vector<Item*>& Enemy::getRewardItems() {return this->rewardItems;}
vector<int>& Enemy::getRewardChances() {return this->rewardChances;}

// setters
void Enemy::setXpToGet(float xpToGet){ this->xpToGet = xpToGet; }

void Enemy::addReward(Item* item, int chance){
	rewardItems.push_back(item);
	rewardChances.push_back(chance);
}

//reset
void Enemy::reset() {
	this->setCurrentHp(this->getMaxHp());
}

// output
ostream& operator<<(ostream& output, Enemy enemy){
	output << enemy.getName() << endl;
	output << " -> Vitality: " << enemy.getCurrentHp() << "/" << enemy.getMaxHp() << " hp" << endl;
	output << " -> Wisdom: " << enemy.getLevel() << ". level" << endl;
	output << " -> Agility: " << enemy.getSpeed() << endl;
	return output;
}
