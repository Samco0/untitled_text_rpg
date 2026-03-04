#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include "item.h"

// Items that restore HP
class Consumable : public Item {
public:
	// Constructors
	Consumable();
	Consumable(string name, string description, float hpToRecover);
	
	// Getter
	float getHpToRecover();
	
	// Setter
	void setHpToRecover(float hpToRecover);
	
	// Output
	friend ostream& operator<<(ostream& output, Consumable c);
	
private:
	float hpToRecover; // how much HP this item restores
};

#endif
