#ifndef GROUPCOMBATLOCATION_H
#define GROUPCOMBATLOCATION_H
#include "combatlocation.h"

class GroupCombatLocation : public CombatLocation {
public:
	GroupCombatLocation();
	GroupCombatLocation(string name, vector<Enemy*> enemies);
	
	vector<Enemy*>& getEnemies();
	bool getIsEmpty();
	
	void setEnemies(vector<Enemy*> enemies);
	void setIsEmpty(bool isEmpty);
	
	void addEnemy(Enemy* enemy);
	void removeEnemy(int index);
private:
	vector<Enemy*> enemies;
	bool isEmpty = true;
};

#endif
