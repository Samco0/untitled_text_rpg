#include "enemy.h"

//constructor
Enemy::Enemy(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = this->maxHp;
	this->maxMana = 0;
	this->currentMana = this->maxMana;
	this->dmg = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
}
Enemy::Enemy(string name, float maxHp, float maxMana, float dmg, int speed, int critChance, float critValue){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = this->maxHp;
	this->maxMana = maxMana;
	this->currentMana = this->maxMana;
	this->dmg = dmg;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
}

//getters
string Enemy::getName(){return this->name;}
float Enemy::getMaxHp(){return this->maxHp;}
float Enemy::getCurrentHp(){return this->currentHp;}
float Enemy::getMaxMana(){return this->maxMana;}
float Enemy::getCurrentMana(){return this->currentMana;}
float Enemy::getDmg(){return this->dmg;}
int Enemy::getSpeed(){return this->speed;}
int Enemy::getCritChance(){return this->critChance;}
float Enemy::getCritValue(){return this->critValue;}

//setters
void Enemy::setName(string name){this->name = name;}
void Enemy::setMaxHp(float maxHp){this->maxHp = maxHp;}
void Enemy::setCurrentHp(float currentHp){
	if(currentHp < 0) this->currentHp = 0;
	else this->currentHp = currentHp;
}
void Enemy::setMaxMana(float maxMana){this->maxMana = maxMana;}
void Enemy::setCurrentMana(float currentMana){
	if(currentMana < 0) this->currentMana = 0;
	else this->currentMana = currentMana;
}
void Enemy::setDmg(float dmg){this->dmg = dmg;}
void Enemy::setSpeed(int speed){this->speed = speed;}
void Enemy::setCritChance(int critChance){this->critChance = critChance;}
void Enemy::setCritValue(float getCritValue){this->critValue = critValue;}
