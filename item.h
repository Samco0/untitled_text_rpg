#ifndef ITEM_H
#define ITEM_H
#include <iostream>
using namespace std;

class Item {
public:
	virtual ~Item() {}
	
	//constructors
	Item();
	Item(string name, string description);
	
	//getters
	string getName();
	string getDescription();
	
	//setters
	void setName(string name);
	void setDescription(string description);
protected:
	string name, description;
};

#endif
