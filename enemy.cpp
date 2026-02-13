#include "enemy.h"

//constructor
Enemy::Enemy(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = this->maxHp;
	this->dmg = 0;
	this->level = 0;
	this->difficulty = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	
	for(int i=0;i<4;i++){
		this->spells[i] = nullptr;
	}
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
	}
}
Enemy::Enemy(string name, float maxHp, float dmg, int level, int difficulty, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = this->maxHp;
	this->dmg = dmg;
	this->level = level;
	this->difficulty = difficulty;
	this->xpToGet = this->level * (this->difficulty*14);
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
	}
}

//getters
float Enemy::getXpToGet(){return this->xpToGet;}
int Enemy::getDifficulty(){return this->difficulty;}

//setters
void Enemy::setXpToGet(float xpToGet){this->xpToGet = xpToGet;}
void Enemy::setDifficulty(int difficulty){this->difficulty = difficulty;}

