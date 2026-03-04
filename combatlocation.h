#ifndef COMBATLOCATION_H
#define COMBATLOCATION_H

#include "location.h"
#include "enemy.h"

// Location where combat happens
class CombatLocation : public Location {
public:
	// Constructors
	CombatLocation();
	CombatLocation(string name, Enemy* attacker);
	
	// Getter
	Enemy* getAttacker();
	
	// Setter
	void setAttacker(Enemy* attacker);
	
private:
	Enemy* attacker; // the enemy waiting in this location
};

#endif
