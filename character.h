#ifndef CHARACTER_H
#define CHARACTER_H
#include <iostream>

using namespace std;

class Character {
public:
	//constructor
	Character();
	Character(string name);
	
	//getters
	string getName();
	
	//setters
	void setName(string name);
protected:
	string name;
};

#endif
