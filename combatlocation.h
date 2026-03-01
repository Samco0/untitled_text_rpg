#ifndef COMBATLOCATION_H
#define COMBATLOCATION_H
#include "location.h"
#include "enemy.h"

class CombatLocation : public Location {
public:
	//constructors
	CombatLocation();
	CombatLocation(string name, Enemy* attacker);
	
	//getters
	Enemy* getAttacker();
	
	//setters
	void setAttacker(Enemy* attacker);
private:
	Enemy* attacker;
};

#endif
