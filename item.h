#ifndef ITEM_H
#define ITEM_H

#include <iostream>
using namespace std;

// Base class for all items
class Item {
public:
	virtual ~Item() {}
	
	// Constructors
	Item();
	Item(string name, string description);
	
	// Getters
	string getName();
	string getDescription();
	
	// Setters
	void setName(string name);
	void setDescription(string description);
protected:
	string name;        // item name
	string description; // item description
};

#endif
