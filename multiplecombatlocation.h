#ifndef MULTIPLECOMBATLOCATION_H
#define MULTIPLECOMBATLOCATION_H

#include "combatlocation.h"

class MultipleCombatLocation : public CombatLocation {
public:
	MultipleCombatLocation();
	MultipleCombatLocation(string name, vector<CombatLocation*> locations);
	
	vector<CombatLocation*>& getLocations();
	bool getIsEmpty();
	
	void setLocations(vector<CombatLocation*> locations);
	void setIsEmpty(bool isEmpty);
	
	void addLocation(CombatLocation* location);
	void deleteLocation(int index);
private:
	vector<CombatLocation*> locations;
	bool isEmpty = true;
};

#endif
