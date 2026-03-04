#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>

using namespace std;

// Base class representing any character in the game.
// Stores only basic identity data.
class Character {
public:
	// Constructors
	Character();
	Character(string name);
	
	// Returns character name
	string getName();
	
	// Sets character name
	void setName(string name);
	
protected:
	string name; // character's name
};

#endif
