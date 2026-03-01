#ifndef LOCATION_H
#define LOCATION_H
#include <iostream>
using namespace std;

class Location {
public:
	virtual ~Location() {}
	
	//constructors
	Location();
	Location(string name);
	
	//getters
	string getName();
	
	//setters
	void setName(string name);
protected:
	string name;
};

#endif
