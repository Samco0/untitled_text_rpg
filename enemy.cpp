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
}

//getters
string Enemy::getName(){return this->name;}
float Enemy::getMaxHp(){return this->maxHp;}
float Enemy::getCurrentHp(){return this->currentHp;}
float Enemy::getXpToGet(){return this->xpToGet;}
int Enemy::getLevel(){return this->level;}
int Enemy::getDifficulty(){return this->difficulty;}
float Enemy::getDmg(){return this->dmg;}
int Enemy::getSpeed(){return this->speed;}
int Enemy::getCritChance(){return this->critChance;}
float Enemy::getCritValue(){return this->critValue;}
Spell** Enemy::getSpells(){return this->spells;}

//setters
void Enemy::setName(string name){this->name = name;}
void Enemy::setMaxHp(float maxHp){this->maxHp = maxHp;}
void Enemy::setCurrentHp(float currentHp){
	if(currentHp < 0) this->currentHp = 0;
	else this->currentHp = currentHp;
}
void Enemy::setXpToGet(float xpToGet){this->xpToGet = xpToGet;}
void Enemy::setLevel(int level){this->level = level;}
void Enemy::setDifficulty(int difficulty){this->difficulty = difficulty;}
void Enemy::setDmg(float dmg){this->dmg = dmg;}
void Enemy::setSpeed(int speed){this->speed = speed;}
void Enemy::setCritChance(int critChance){this->critChance = critChance;}
void Enemy::setCritValue(float getCritValue){this->critValue = critValue;}
void Enemy::setSpells(Spell* spells[4]){
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
}
