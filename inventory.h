#ifndef INVENTORY_H
#define INVENTORY_H
#include "weapon.h"
#include "scroll.h"
#include <vector>

class Inventory {
public:
	//constructors
	Inventory();
	Inventory(vector<Item*> storage);
	
	//getters
	vector<Item*>& getStorage();
	
	//setter
	void setStorage(vector<Item*> storage);
	
	//manipulation with items
	void addItem(Item* item);
	void removeItem(int index);
	
	friend ostream& operator<<(ostream& os, Inventory inv);
private:
	vector<Item*> storage;
};

#endif
