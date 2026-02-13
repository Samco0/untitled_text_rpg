#ifndef ITEM_H
#define ITEM_H
#include <iostream>
using namespace std;

class Item {
public:
	//constructors
	Item();
	Item(string name, string description);
private:
	string name, description;
};

#endif
