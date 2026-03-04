#ifndef PLAYER_H
#define PLAYER_H

// Prevents multiple inclusion of this header file
#include <cmath>
#include "combatcharacter.h"
#include "chainingspell.h"
#include "statuseffect.h"
#include "inventory.h"

// Player class inherits from CombatCharacter
class Player : public CombatCharacter {
public:
	
	// ===== Constructors =====
	
	Player(); // Default constructor
	
	// Full constructor for initializing player with stats, weapon and spells
	Player(string name, float maxHp, float dmg, int balance, int speed, int critChance, float critValue, Weapon* weapon, Spell* spells[4]);
	
	
	// ===== Getters =====
	
	float getRequireXp();      // XP required for next level
	float getCurrentXp();      // Current XP
	Weapon* getWeapon();       // Equipped weapon
	Inventory& getInventory(); // Player inventory
	
	// Equipped armor pieces
	Armor* getHelmet();
	Armor* getChestplate();
	Armor* getGloves();
	Armor* getLeggings();
	Armor* getBoots();
	
	
	// ===== Setters =====
	
	void setRequireXp(float requireXp);
	void setCurrentXp(float currentXp);
	void setWeapon(Weapon* weapon);
	void setInventory(Inventory inventory);
	
	void setHelmet(Armor* helmet);
	void setChestplate(Armor* chestplate);
	void setGloves(Armor* gloves);
	void setLeggings(Armor* leggings);
	void setBoots(Armor* boots);
	
	
	// Allows printing Player object using cout
	friend ostream& operator<<(ostream& output, Player player);
	
protected:
	
	// ===== Experience system =====
	
	float requireXp;  // XP needed for next level
	float currentXp;  // Current XP amount
	
	/*
	Leveling formula:
	Level 1 -> 2 requires 15 XP
	Each next level requires previous XP * 1.4
	*/
	
	
	// ===== Equipment & Inventory =====
	
	Weapon* weapon;     // Currently equipped weapon
	Inventory inventory; // Player inventory
	
	// Equipped armor slots
	Armor* helmet;
	Armor* chestplate;
	Armor* gloves;
	Armor* leggings;
	Armor* boots;
};

#endif
