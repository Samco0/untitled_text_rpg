#include <iostream>
#include <ctime>
#include <cstdlib>
#include "battle.h"
#include "chainingspell.h"
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	int randomiseAttackPlayer, randomiseAttackEnemy;
	int randomiseSpellPlayer, randomiseSpellEnemy;
	
	Spell sp1("Fireball", "A ball of fire lol", 5, 3);
	Spell sp2("Frostball", "A ball of frost lol", 4, 3);
	ChainingSpell sp3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	Spell* s1[4] = { &sp1, &sp2, &sp3, nullptr };
	
	Spell se1("Fireball", "A ball of fire lol", 5, 3);
	Spell se2("Frostball", "A ball of frost lol", 4, 3);
	ChainingSpell se3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	Spell* s2[4] = { &se1, &se2, &se3, nullptr };
	
	Player p1("Samqo", 20, 10, 2, 0, 4, 20, 1.2, s1);
	Enemy e1("Ciganek", 20, 10, 1, 3, 10, 3, s2);
	
	Battle b1(&p1, &e1);
	
	while(b1.getIsFinished() == 0){
		randomiseAttackPlayer = rand() % 10;
		randomiseAttackEnemy = rand() % 10;
		randomiseSpellPlayer = rand() % 4;
		randomiseSpellEnemy = rand() % 4;
		
		cout << p1.getName() << "'s Hp: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << endl;
		cout << "--------------------------------------" << endl;
		cout << e1.getName() << "'s Hp: " << e1.getCurrentHp() << "/" << e1.getMaxHp() << endl;
		cout << "--------------------------------------" << endl << endl;
		
		if(p1.getSpeed() >= e1.getSpeed()){
			Spell* ps = p1.getSpells()[randomiseSpellPlayer];
			if(randomiseAttackPlayer < 7 || ps == nullptr || ps->getRemainingCooldown() > 0){
				b1.attackEnemy();
			} else {
				b1.useSpellOnEnemy(randomiseSpellPlayer);
			}
			
			Spell* es = e1.getSpells()[randomiseSpellEnemy];
			if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
				b1.attackPlayer();
			} else {
				b1.useSpellOnPlayer(randomiseSpellEnemy);
			}
			
		} else {
			Spell* es = e1.getSpells()[randomiseSpellEnemy];
			if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
				b1.attackPlayer();
			} else {
				b1.useSpellOnPlayer(randomiseSpellEnemy);
			}
			
			Spell* ps = p1.getSpells()[randomiseSpellPlayer];
			if(randomiseAttackPlayer < 7 || ps == nullptr || ps->getRemainingCooldown() > 0){
				b1.attackEnemy();
			} else {
				b1.useSpellOnEnemy(randomiseSpellPlayer);
			}
		}
		
		b1.roundIncrement();
		b1.checkDeaths();
		cout << "--------------------------------------" << endl;
	}
	
	return 0;
}

