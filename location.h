#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>
using namespace std;

// Base class for any place in the game
class Location {
public:
	virtual ~Location() {}
	
	// Constructors
	Location();
	Location(string name);
	
	// Getter
	string getName();
	
	// Setter
	void setName(string name);
	
protected:
	string name; // location's name
};

#endif
