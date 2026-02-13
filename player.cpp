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
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
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
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
	}
}

//getters
float Player::getRequireXp(){return this->requireXp;}
float Player::getCurrentXp(){return this->currentXp;}
int Player::getBalance(){return this->balance;}

//setters
void Player::setRequireXp(float requireXp){this->requireXp = requireXp;}
void Player::setCurrentXp(float currentXp){
	this->currentXp += currentXp; 
	
	while (this->currentXp >= this->requireXp){
		this->currentXp -= this->requireXp;
		this->level++;
		
		cout << " -> You (" << this->getName() << ") achieved level " << this->level << "." << endl;
		this->requireXp *= 1.4f;
	}
	
	cout << "==========================================" << endl;
}
void Player::setBalance(int balance){this->balance = balance;}
