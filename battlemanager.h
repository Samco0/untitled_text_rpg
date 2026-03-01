#ifndef BATTLE_H
#define BATTLE_H
#include <iostream>
#include "player.h"
#include "enemy.h"
#include "hpstatuseffect.h"
#include "statuseffectspell.h"
#include "hpstatuseffect.h"

using namespace std;

//this class is a solution to an infinite include loop
//it basically works as the entierity of battle system

class BattleManager {
public:
	//constructors
	BattleManager();
	BattleManager(CombatCharacter* player, CombatCharacter* enemy);
	
	//getters
	int getRound();
	int getIsFinished();
	CombatCharacter* getPlayer();
	CombatCharacter* getEnemy();
	
	//setters
	void setRound(int round);
	void setIsFinished(int isFinished);
	void setPlayer(CombatCharacter* player);
	void setEnemy(CombatCharacter* enemy);
	
	//battle methods
	void attack(CombatCharacter* attacker, CombatCharacter* target);
	void attackUsingSpell(CombatCharacter* attacker, CombatCharacter* target, int spellIndex);
	
	//battle misc functions
	void roundIncrement();
	void checkDeaths();
	void checkStatusEffects();
	
	//the battle itself
	void battle();
private:
	int round;
	int isFinished; //this helps to determine, whether the battle already ended or not; 0 - still continues; 1 - enemy won; 2 - player won
	CombatCharacter* player;
	CombatCharacter* enemy;
};

#endif
