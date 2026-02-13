#include <iostream>
#include <ctime>
#include <cstdlib>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	HpStatusEffect hps1("Burning", 5, 1, -5);
	
	Spell sp1("Shadow claw", "Claw of shadow", 10, 3);
	Spell sp2("Frostball", "A ball of frost lol", 4, 3);
	ChainingSpell sp3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	StatusEffectSpell sp4("Dragons breath", "From your mouth comes out a flame.", 5, 5, 80, &hps1);
	Spell* s1[4] = { &sp1, &sp2, &sp3, &sp4 };
	
	Spell se1("Jewball", "A ball of fire lol", 5, 3);
	Spell se2("Energy Surge", "Energy surge lol", 4, 3);
	ChainingSpell se3("Coin Rush", "Barrage of coins", 2.5, 5, 1, 6);
	Spell* s2[4] = { &se1, &se2, &se3, nullptr };
	
	Spell se4("Jewish Ripper", "Jews", 7, 3);
	ChainingSpell se5("Gold Rush", "Jews", 3, 5, 1, 6);
	Spell* s3[4] = { &se4, &se5, nullptr, nullptr };
	
	Player p1("Samqo", 30, 2, 0, 4, 20, 1.2, s1);
	Enemy e1("Terezka Zidova", 15, 1, 1, 1, 5, 10, 3, s2);
	Enemy e2("Marecek Zidacek", 25, 1.5, 1, 2, 4, 12.5, 2.5, s3);
	
	Enemy* enemies[] = {&e1, &e2};
	
	for(int i=0;i<2;i++){
		BattleManager b(&p1,enemies[i]);
		b.battle();
	}
	
	
	return 0;
}

