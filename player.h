#ifndef PLAYER_H
#define PLAYER_H

#include <cmath>
#include "combatcharacter.h"
#include "chainingspell.h"
#include "statuseffect.h"
#include "inventory.h"

class Player : public CombatCharacter {
public:
	Player();
	Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Weapon* weapon, Spell* spells[4]);
	
	// Getters
	float getRequireXp();
	float getCurrentXp();
	Weapon* getWeapon();
	Inventory& getInventory();
	Armor* getHelmet();
	Armor* getChestplate();
	Armor* getGloves();
	Armor* getLeggings();
	Armor* getBoots();
	int getSoulStones();
	
	// Setters
	void setRequireXp(float requireXp);
	void setCurrentXp(float currentXp);
	void setWeapon(Weapon* weapon);
	void setInventory(Inventory inventory);
	void setHelmet(Armor* helmet);
	void setChestplate(Armor* chestplate);
	void setGloves(Armor* gloves);
	void setLeggings(Armor* leggings);
	void setBoots(Armor* boots);
	void setSoulStones(int n);
	void addSoulStone();     // grants +1, capped at 9
	bool useSoulStone();     // consumes 1, returns false if none left
	
	friend ostream& operator<<(ostream& output, Player player);
	
protected:
	float requireXp;
	float currentXp;
	Weapon* weapon;
	Inventory inventory;
	Armor* helmet;
	Armor* chestplate;
	Armor* gloves;
	Armor* leggings;
	Armor* boots;
	int soulStones;
};

#endif
