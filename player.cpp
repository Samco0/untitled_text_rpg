#include "player.h"

Player::Player(){
	this->name = "";
	this->maxHp = 0;
	this->currentHp = 0;
	this->dmg = 0;
	this->currentXp = 0;
	this->requireXp = 15;
	this->level = 1;
	this->speed = 0;
	this->critChance = 0;
	this->critValue = 0;
	this->weapon = nullptr;
	this->soulStones = 0;
	for(int i=0;i<4;i++) this->spells[i] = nullptr;
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
	this->helmet = nullptr;
	this->chestplate = nullptr;
	this->gloves = nullptr;
	this->leggings = nullptr;
	this->boots = nullptr;
}

Player::Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Weapon* weapon, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = maxHp;
	this->dmg = dmg;
	this->currentXp = 0;
	this->requireXp = 15;
	this->level = 1;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	this->weapon = weapon;
	this->soulStones = 0;
	for(int i=0;i<4;i++) this->spells[i] = spells[i];
	for(int i=0;i<4;i++) this->statusEffect[i] = nullptr;
	this->helmet = nullptr;
	this->chestplate = nullptr;
	this->gloves = nullptr;
	this->leggings = nullptr;
	this->boots = nullptr;
}

// Getters
float Player::getRequireXp(){ return this->requireXp; }
float Player::getCurrentXp(){ return this->currentXp; }
Weapon* Player::getWeapon(){ return this->weapon; }
Inventory& Player::getInventory(){ return this->inventory; }
Armor* Player::getHelmet(){ return this->helmet; }
Armor* Player::getChestplate(){ return this->chestplate; }
Armor* Player::getGloves(){ return this->gloves; }
Armor* Player::getLeggings(){ return this->leggings; }
Armor* Player::getBoots(){ return this->boots; }
int Player::getSoulStones(){ return this->soulStones; }

// Setters
void Player::setRequireXp(float requireXp){ this->requireXp = requireXp; }
void Player::setCurrentXp(float xpGained){
	this->currentXp += xpGained;
	while (this->currentXp >= this->requireXp){
		this->currentXp -= this->requireXp;
		this->level++;
		this->maxHp *= 1.20f;
		this->maxHp = std::round(this->maxHp * 10.0f) / 10.0f;
		this->dmg *= 1.07f;
		this->dmg = std::round(this->dmg * 10) / 10;
		this->currentHp = this->maxHp;
		this->requireXp *= 1.8f;
		cout << "==========================================" << endl;
		cout << " -> Power surges through your veins." << endl;
		cout << " -> " << this->getName() << " ascends to level " << this->level << "." << endl;
		cout << " -> Vitality restored." << endl;
		cout << "==========================================" << endl;
	}
}
void Player::setWeapon(Weapon* weapon){ this->weapon = weapon; }
void Player::setInventory(Inventory inventory){ this->inventory = inventory; }
void Player::setHelmet(Armor* helmet){ if(helmet && helmet->getType() == 1) this->helmet = helmet; }
void Player::setChestplate(Armor* chestplate){ if(chestplate && chestplate->getType() == 2) this->chestplate = chestplate; }
void Player::setGloves(Armor* gloves){ if(gloves && gloves->getType() == 3) this->gloves = gloves; }
void Player::setLeggings(Armor* leggings){ if(leggings && leggings->getType() == 4) this->leggings = leggings; }
void Player::setBoots(Armor* boots){ if(boots && boots->getType() == 5) this->boots = boots; }
void Player::setSoulStones(int n){ this->soulStones = n; }

void Player::addSoulStone(){
	if (this->soulStones < 9) this->soulStones++;
}

bool Player::useSoulStone(){
	if (this->soulStones > 0){
		this->soulStones--;
		return true;
	}
	return false;
}

// Output
ostream& operator<<(ostream& output, Player player){
	output << player.getName() << endl;
	output << " -> Vitality:    " << player.getCurrentHp() << "/" << player.getMaxHp() << " hp" << endl;
	output << " -> Wisdom:      level " << player.getLevel() << endl;
	output << " -> Agility:     " << player.getSpeed() << endl;
	
	output << " -> Soul Stones: ";
	if (player.getSoulStones() == 0){
		output << "none  (die again and it ends here)";
	} else {
		for(int i = 0; i < player.getSoulStones(); i++) output << "[*] ";
	}
	output << endl;
	return output;
}
