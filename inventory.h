#ifndef INVENTORY_H
#define INVENTORY_H
#include "weapon.h"
#include "scroll.h"
#include "armor.h"
#include "consumable.h"
#include "attackgadget.h"

#include <vector>
#include <iostream>
using namespace std;

// Inventory class, holds items
class Inventory {
public:
	// Constructors
	Inventory();
	Inventory(vector<Item*> storage);
	
	// Getter
	vector<Item*>& getStorage();
	
	// Setter
	void setStorage(vector<Item*> storage);
	
	// Add an item
	void addItem(Item* item);
	
	// Output
	friend ostream& operator<<(ostream& os, Inventory inv);
	
private:
	vector<Item*> storage; // all items in inventory
};

#endif
