#ifndef ARMOR_H
#define ARMOR_H

#include "item.h"

// Armor class inherits from Item
class Armor : public Item {
public:
	
	// ===== Constructors =====
	
	Armor(); // Default constructor
	
	// Initializes armor with name, description, damage reduction value and type
	Armor(string name, string description, float damageReduction, int type);
	
	
	// ===== Getters =====
	
	float getDamageReduction(); // Returns damage reduction value
	int getType();              // Returns armor type (e.g. helmet, chestplate, etc.)
	
	
	// ===== Setters =====
	
	void setDamageReduction(float damageReduction);
	void setType(int type);
	
	
	// Allows printing Armor object using cout
	friend ostream& operator<<(ostream& output, Armor a);
	
private:
	
	float damageReduction; // Percentage of damage reduction
	int type;              // Armor slot/type identifier
};

#endif
