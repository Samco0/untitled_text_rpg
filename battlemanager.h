#ifndef BATTLE_H
#define BATTLE_H

#include <iostream>
#include <cmath>
#include "player.h"
#include "enemy.h"
#include "hpstatuseffect.h"
#include "armorbreakstatuseffect.h"
#include "tauntstatuseffect.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"

using namespace std;

/*
BattleManager handles the entire combat logic.
It was also created to avoid circular include problems.
This class controls turns, attacks, spells and win conditions.
*/

class BattleManager {
public:
	// ===== Constructors =====
	BattleManager();
	BattleManager(CombatCharacter* player, CombatCharacter* enemy);
	
	// ===== Getters =====
	int getRound();                 // current battle round
	int getIsFinished();            // battle state (0 = ongoing, 1 = enemy won, 2 = player won)
	CombatCharacter* getPlayer();   // pointer to player
	CombatCharacter* getEnemy();    // pointer to enemy
	
	// ===== Setters =====
	void setRound(int round);
	void setIsFinished(int isFinished);
	void setPlayer(CombatCharacter* player);
	void setEnemy(CombatCharacter* enemy);
	
	// ===== Core battle mechanics =====
	void attack(CombatCharacter* attacker, CombatCharacter* target); 
	// performs basic attack
	
	void attackUsingSpell(CombatCharacter* attacker, CombatCharacter* target, int spellIndex); 
	// performs spell attack based on selected spell index
	
	// ===== Battle flow control =====
	void roundIncrement();   // moves battle to next round
	void checkDeaths();      // checks if someone died
	void checkStatusEffects(); // applies and updates status effects
	
	// Starts and controls the whole battle loop
	void battle();
	
private:
	int round;        // current round number
	
	/*
	0 - battle continues
	1 - enemy won
	2 - player won
	*/
	int isFinished;
	
	CombatCharacter* player; // pointer to player entity
	CombatCharacter* enemy;  // pointer to enemy entity
};

#endif
