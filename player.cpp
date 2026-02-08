#include "player.h"

//constructor
Player::Player(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = this->maxHp;
	this->maxMana = 0;
	this->currentMana = this->maxMana;
	this->dmg = 0;
	this->balance = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	
	for(int i=0;i<4;i++){
		this->spells[i] = nullptr;
	}
}
Player::Player(string name, float maxHp, float maxMana, float dmg, int balance, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = this->maxHp;
	this->maxMana = maxMana;
	this->dmg = dmg;
	this->balance = balance;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
}

//getters
float Player::getMaxHp(){return this->maxHp;}
float Player::getCurrentHp(){return this->currentHp;}
float Player::getMaxMana(){return this->maxMana;}
float Player::getCurrentMana(){return this->currentMana;}
float Player::getDmg(){return this->dmg;}
int Player::getBalance(){return this->balance;}
int Player::getSpeed(){return this->speed;}
int Player::getCritChance(){return this->critChance;}
float Player::getCritValue(){return this->critValue;}
Spell** Player::getSpells(){return this->spells;}

//setters
void Player::setMaxHp(float maxHp){this->maxHp = maxHp;}
void Player::setCurrentHp(float currentHp){
	if(currentHp < 0) this->currentHp = 0;
	else this->currentHp = currentHp;
}
void Player::setMaxMana(float maxMana){this->maxMana = maxMana;}
void Player::setCurrentMana(float currentMana){
	if(currentMana < 0) this->currentMana = 0;
	else this->currentMana = currentMana;
}
void Player::setDmg(float dmg){this->dmg = dmg;}
void Player::setBalance(int balance){this->balance = balance;}
void Player::setSpeed(int speed){this->speed = speed;}
void Player::setCritChance(int critChance){this->critChance = critChance;}
void Player::setCritValue(float getCritValue){this->critValue = critValue;}
void Player::setSpells(Spell* spells[4]){
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
}
