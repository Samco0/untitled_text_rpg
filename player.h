#ifndef PLAYER_H
#define PLAYER_H
#include <cmath>
#include "combatcharacter.h"
#include "chainingspell.h"
#include "statuseffect.h"
#include "inventory.h"

class Player : public CombatCharacter{
public:
	//constructor
	Player();
	Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Weapon* weapon, Spell* spells[4]);
	
	//getters
	float getRequireXp();
	float getCurrentXp();
	int getBalance();
	Weapon* getWeapon();
	Inventory& getInventory();
	Armor* getHelmet();
	Armor* getChestplate();
	Armor* getGloves();
	Armor* getLeggings();
	Armor* getBoots();
	
	//setters
	void setRequireXp(float requireXp);
	void setCurrentXp(float currentXp);
	void setBalance(int balance);
	void setWeapon(Weapon* weapon);
	void setInventory(Inventory inventory);
	void setHelmet(Armor* helmet);
	void setChestplate(Armor* chestplate);
	void setGloves(Armor* gloves);
	void setLeggings(Armor* leggings);
	void setBoots(Armor* boots);
	
	//output
	friend ostream& operator<<(ostream& output, Player player);
protected:
	float requireXp, currentXp;
	/*
	from level 1 to level 2 it requires 15xp
	for each next level it requires the previous amount x1.4
	*/
	int balance;
	Weapon* weapon;
	Inventory inventory;
	
	Armor* helmet;
	Armor* chestplate;
	Armor* gloves;
	Armor* leggings;
	Armor* boots;
};

#endif
