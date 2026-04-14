#include "groupcombatlocation.h"

GroupCombatLocation::GroupCombatLocation(){
	this->name = "";
}
GroupCombatLocation::GroupCombatLocation(string name, vector<Enemy*> enemies){
	this->name = name;
	this->enemies = enemies;
}

vector<Enemy*>& GroupCombatLocation::getEnemies(){ return this->enemies; }
bool GroupCombatLocation::getIsEmpty(){ return this->isEmpty; }

void GroupCombatLocation::setEnemies(vector<Enemy*> enemies){ this->enemies = enemies; }
void GroupCombatLocation::setIsEmpty(bool isEmpty){ this->isEmpty = isEmpty; }

void GroupCombatLocation::addEnemy(Enemy* enemy){ if(enemy) this->enemies.push_back(enemy); }
void GroupCombatLocation::removeEnemy(int index){
	if(index < 0 || index >= (int)this->enemies.size()) return;
	this->enemies.erase(this->enemies.begin() + index);
}
