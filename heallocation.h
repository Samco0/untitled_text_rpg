#ifndef HEALLOCATION_H
#define HEALLOCATION_H
#include "location.h"

class HealLocation : public Location {
public:
	HealLocation();
	HealLocation(string name, string healName);
	
	string getHealName();
	
	void setHealName(string healName);
private:
	string healName;
};

#endif
