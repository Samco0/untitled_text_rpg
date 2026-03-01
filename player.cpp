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
	this->weapon = nullptr;
	
	for(int i=0;i<4;i++){
		this->spells[i] = nullptr;
	}
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
	}
	
	this->helmet = nullptr;
	this->chestplate = nullptr;
	this->gloves = nullptr;
	this->leggings = nullptr;
	this->boots = nullptr;
}
Player::Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Weapon* weapon, Spell* spells[4]){
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
	this->weapon = weapon;
	
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
	
	for(int i=0;i<4;i++){
		this->statusEffect[i] = nullptr;
	}
	
	this->helmet = nullptr;
	this->chestplate = nullptr;
	this->gloves = nullptr;
	this->leggings = nullptr;
	this->boots = nullptr;
}

//getters
float Player::getRequireXp(){return this->requireXp;}
float Player::getCurrentXp(){return this->currentXp;}
int Player::getBalance(){return this->balance;}
Weapon* Player::getWeapon(){return this->weapon;}
Inventory& Player::getInventory(){return this->inventory;}
Armor* Player::getHelmet(){return this->helmet;}
Armor* Player::getChestplate(){return this->chestplate;}
Armor* Player::getGloves(){return this->gloves;}
Armor* Player::getLeggings(){return this->leggings;}
Armor* Player::getBoots(){return this->boots;}

//setters
void Player::setRequireXp(float requireXp){this->requireXp = requireXp;}
void Player::setCurrentXp(float currentXp){
	int levelBefore = this->level;
	this->currentXp += currentXp; 
	
	while (this->currentXp >= this->requireXp){
		this->currentXp -= this->requireXp;
		this->level++;
		
		//level increases maxHp
		this->maxHp = this->maxHp * 1.15;
		this->maxHp = std::round(this->maxHp * 10) / 10;
		
		//level increases dmg
		this->dmg = this->dmg * 1.15;
		this->dmg = std::round(this->dmg * 10) / 10;
		
		cout << " -> " << this->getName() << "\'s wisdom rises to level " << this->level << "." << endl;
		this->requireXp *= 1.4f;
	}
	
	if(levelBefore < this->level){
	cout << " -> Your strength grows... the journey continues." << endl;
	cout << "==========================================" << endl;
	}
}
void Player::setBalance(int balance){this->balance = balance;}
void Player::setWeapon(Weapon* weapon){this->weapon = weapon;}
void Player::setInventory(Inventory inventory){this->inventory = inventory;}
void Player::setHelmet(Armor* helmet){if(helmet->getType() == 1) this->helmet = helmet;}
void Player::setChestplate(Armor* chestplate){if(helmet->getType() == 2) this->chestplate = chestplate;}
void Player::setGloves(Armor* gloves){if(helmet->getType() == 3) this->gloves = gloves;}
void Player::setLeggings(Armor* leggings){if(helmet->getType() == 4) this->leggings = leggings;}
void Player::setBoots(Armor* boots){if(helmet->getType() == 5) this->boots = boots;}

//output
ostream& operator<<(ostream& output, Player player){
	cout << player.getName() << endl;
	cout << " -> Vitality: " << player.getCurrentHp() << "/" << player.getMaxHp() << "hp" << endl;
	cout << " -> Wisdom: " << player.getLevel() << ". level"<< endl;
	cout << " -> Agility: " << player.getSpeed() << endl;
	return output;
}

