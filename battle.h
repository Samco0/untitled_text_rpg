#ifndef BATTLE_H
#define BATTLE_H
#include <iostream>
#include "player.h"
#include "enemy.h"

using namespace std;

//this class is a solution to an infinite include loop
//it basically works as the entierity of battle system

class Battle {
public:
	//constructors
	Battle();
	Battle(Player* player, Enemy* enemy);
	
	//getters
	int getRound();
	int getIsFinished();
	Player* getPlayer();
	Enemy* getEnemy();
	
	//setters
	void setRound(int round);
	void setIsFinished(int isFinished);
	void setPlayer(Player* player);
	void setEnemy(Enemy* enemy);
	
	//battle methods
	void attackEnemy();
	void attackPlayer();
	void useSpellOnEnemy(int spellIndex);
	void useSpellOnPlayer(int spellIndex);
	
	//battle misc functions
	void roundIncrement();
	void checkDeaths();
private:
	int round;
	int isFinished; //this helps to determine, whether the battle already ended or not; 0 - still continues; 1 - enemy won; 2 - player won
	Player* player;
	Enemy* enemy;
};

#endif
