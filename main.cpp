#include <iostream>
#include <ctime>
#include <cstdlib>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "map.h"
#include "weapon.h"
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	HpStatusEffect hps1("Burning", 5, 1, -1);
	HpStatusEffect hps2("Carbonmonoxide Poisoning", 7, 3, -1);
	HpStatusEffect hps3("Saturation", 5, 1, 5);
	
	Spell sp1("Shadow claw", "Claw of shadow", 10, 3);
	ChainingSpell sp3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	StatusEffectSpell sp4("Dragons breath", "From your mouth comes out a flame.", 5, 5, 80, &hps1);
	Spell* s1[4] = { &sp1, nullptr, &sp3, &sp4 };
	
	StatusEffectSpell sp5("KEBAB", "A kebab spawns in the air and you eat it.", 0, 5, 80, &hps3);
	Scroll scr1("Idk.", &sp5);
	
	Spell se1("Gob-ball!", "A ball of liquid gold.", 5, 3);
	Spell se2("Energy Surge", "Energy surge.", 4, 3);
	ChainingSpell se3("Coin Rush", "Barrage of coins.", 2.5, 5, 1, 6);
	Spell* s2[4] = { &se1, &se2, &se3, nullptr };
	
	Spell se4("Jewish Ripper", "Jews", 7, 3);
	ChainingSpell se5("Gold Rush", "Jews", 3, 5, 1, 6);
	Spell* s3[4] = { &se4, &se5, nullptr, nullptr };
	
	Spell se6("Chokehold", "Grabs you and chokes you", 5, 5);
	StatusEffectSpell se7("Breath of the Silent Hearth", "The holder breathes out a invisible cloud, that drain oxygen form the opponents lungs", 2.5, 7, 70, &hps2);
	ChainingSpell se8("Wraith of the Crazy Wolf", "The holder goes into a barrage of fists", 1, 5, 3, 6);
	Spell* s4[4] = { &se6, &se7, &se8, nullptr};
	
	Weapon w1("Muramasa", "Sword", "Muramasa is a japanese style katana", 5);
	Weapon w2("Excalibur", "Sword", "The excalibur", 6);
	
	Player p1("Samqo", 55, 2, 0, 4, 20, 1.2, &w1, s1);
	p1.getInventory().addItem(&w2);
	p1.getInventory().addItem(&scr1);
	
	Enemy e1("Gozu the Goblin", 15, 1, 1, 1, 5, 10, 3, s2);
	CombatLocation cl1("Goblinville Street 1", &e1);
	Enemy e2("Maro the Goblin", 25, 1.5, 1, 2, 4, 12.5, 2.5, s3);
	CombatLocation cl2("Goblinville Street 2", &e2);
	Enemy e3("GoblinBeg", 50, 2.25, 2, 3, 7, 15, 1.5, s4);
	CombatLocation cl3("The Gringot's Bank", &e3);
	
	vector<Location*> l = {&cl1, &cl2, &cl3};
	Map m("Reign of the Mighty GoblinBeg", &p1, l);
	
	while(m.getPlayerFinished()==false){
		cout << m;
		m.movePlayer();
	};
	
	return 0;
}
