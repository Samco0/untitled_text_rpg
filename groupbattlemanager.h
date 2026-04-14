#ifndef GROUPBATTLEMANAGER_H
#define GROUPBATTLEMANAGER_H

#include <iostream>
#include <vector>
#include <cmath>
#include "player.h"
#include "enemy.h"
#include "hpstatuseffect.h"
#include "armorbreakstatuseffect.h"
#include "tauntstatuseffect.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "chainingspell.h"

using namespace std;

/*
GroupBattleManager handles 1 player vs N enemies combat.
Enemies all act simultaneously each round.
Player chooses a target for each action.
Battle ends (rewards given) only when ALL enemies are defeated.
*/

class GroupBattleManager {
public:
	GroupBattleManager();
	GroupBattleManager(CombatCharacter* player, vector<Enemy*> enemies);
	
	int  getRound();
	int  getIsFinished();
	CombatCharacter*  getPlayer();
	vector<Enemy*>&   getEnemies();
	
	void setRound(int round);
	void setIsFinished(int isFinished);
	void setPlayer(CombatCharacter* player);
	
	// ---- core mechanics (reuse BattleManager logic, adapted for group) ----
	void playerAttack(Enemy* target);
	void playerAttackUsingSpell(Enemy* target, int spellIndex);
	void playerUseItem(int itemIndex, Enemy* target);   // consumable heals player; gadget hits target
	
	void enemyTakeTurn(Enemy* e);   // single enemy decides basic or spell, hits player
	
	// ---- round flow ----
	void allEnemiesAct();           // every living enemy takes their turn
	void roundIncrement();          // spell CDs, status effect rounds
	void checkStatusEffects();      // apply DOT / remind armor break / taunt per character
	void checkDeaths();             // remove dead enemies; set isFinished if player died or all enemies dead
	void giveRewards();             // XP + loot from all enemies combined
	
	void battle();                  // main loop
	
private:
	int round;
	/*
	0 = ongoing
	1 = player lost
	2 = player won
	*/
	int isFinished;
	
	CombatCharacter* player;
	vector<Enemy*>   enemies;   // all enemies in the group; dead ones get HP clamped to 0 and are skipped
	
	// ---- taunt state ----
	// if non-null, player basic attack and spells are forced toward this enemy
	Enemy* tauntTarget = nullptr;
	int    tauntRoundsLeft = 0;
	vector<Enemy*> announcedDeaths;  // tracks which enemies have been announced dead
	
	// ---- helpers ----
	void printBattleState();
	void applyArmorReduction(float& dmg, Player* target);
	bool isArmorBroken(Player* target);
	vector<Enemy*> livingEnemies();
	int  pickTarget(const string& prompt);   // shows numbered list, returns index into enemies[]
	string conjureMessage(CombatCharacter* attacker, const string& spellType);
};

#endif
