#include "player.h"

//constructor
Player::Player(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = this->maxHp;
	this->dmg = 0;
	this->currentXp = 0;
	this->requireXp = 15;
	this->level = 1;
	this->balance = 0;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	
	for(int i=0;i<4;i++){
		this->spells[i] = nullptr;
	}
}
Player::Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = this->maxHp;
	this->dmg = dmg;
	this->currentXp = 0;
	this->requireXp = 15;
	this->level = 1;
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
float Player::getDmg(){return this->dmg;}
float Player::getRequireXp(){return this->requireXp;}
float Player::getCurrentXp(){return this->currentXp;}
int Player::getLevel(){return this->level;}
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
void Player::setDmg(float dmg){this->dmg = dmg;}
void Player::setRequireXp(float requireXp){this->requireXp = requireXp;}
void Player::setCurrentXp(float currentXp){
	this->currentXp += currentXp; 
	
	while (this->currentXp >= this->requireXp){
		this->currentXp -= this->requireXp;
		this->level++;
		
		cout << " -> You (" << this->getName() << ") achieved level " << this->level << "." << endl;
		this->requireXp *= 1.4f;
	}
}

void Player::setLevel(int level){this->level = level;}
void Player::setBalance(int balance){this->balance = balance;}
void Player::setSpeed(int speed){this->speed = speed;}
void Player::setCritChance(int critChance){this->critChance = critChance;}
void Player::setCritValue(float getCritValue){this->critValue = critValue;}
void Player::setSpells(Spell* spells[4]){
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
}
