#include <iostream>
#include <ctime>
#include <cstdlib>
#include "battle.h"
#include "chainingspell.h"
using namespace std;

int main(int argc, char** argv) {
	srand(time(NULL));
	
	int randomiseAttackEnemy;
	int randomiseSpellEnemy;
	int choice = 0;
	
	Spell sp1("Jewish obliterator", "A ball of fire lol", 1000, 3);
	Spell sp2("Frostball", "A ball of frost lol", 4, 3);
	ChainingSpell sp3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	Spell* s1[4] = { &sp1, &sp2, &sp3, nullptr };
	
	Spell se1("Jewball", "A ball of fire lol", 5, 3);
	Spell se2("Bicycle stealing ball", "A ball of frost lol", 4, 3);
	ChainingSpell se3("Coin rush", "Barrage of thunders", 2.5, 5, 1, 6);
	Spell* s2[4] = { &se1, &se2, &se3, nullptr };
	
	Player p1("Samqo", 20, 2, 0, 4, 20, 1.2, s1);
	Enemy e1("Terezka", 20, 1, 5, 10, 3, s2);
	
	Battle b1(&p1, &e1);
	
	while(b1.getIsFinished() == 0){
		randomiseAttackEnemy = rand() % 10;
		randomiseSpellEnemy = rand() % 4;
		
		cout << "==========================================" << endl;
		cout << p1.getName() << endl;
		cout << " -> Health: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << "hp" << endl;
		cout << " -> Speed:  " << p1.getSpeed() << endl;
		cout << "------------------------------------------" << endl;
		cout << e1.getName() << endl;
		cout << " -> Health: " << e1.getCurrentHp() << "/" << e1.getMaxHp() << "hp" << endl;
		cout << " -> Speed:  " << e1.getSpeed() << endl;
		cout << "==========================================" << endl;
		cout << "Your Options:" << endl;
		cout << " 1. Basic attack" << endl;
		cout << " 2. Use spell" << endl;
		cout << "------------------------------------------" << endl;
		cout << "What do you plan on doing adventurer? ";
		cin >> choice;
		cout << "==========================================" << endl << endl;
		system("cls");
		
		switch (choice) {
		case 1:
			if(p1.getSpeed() >= e1.getSpeed()){
				cout << "==========================================" << endl;
				b1.attackEnemy();
				
				cout << endl;
				
				Spell* es = e1.getSpells()[randomiseSpellEnemy];
				if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
					b1.attackPlayer();
				} else {
					b1.useSpellOnPlayer(randomiseSpellEnemy);
				}
				
			} else {
				cout << "==========================================" << endl;
				Spell* es = e1.getSpells()[randomiseSpellEnemy];
				if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
					b1.attackPlayer();
				} else {
					b1.useSpellOnPlayer(randomiseSpellEnemy);
				}
				
				cout << endl;
				
				b1.attackEnemy();
			}
			break;
		case 2:
			cout << "==========================================" << endl;
			cout << p1.getName() << endl;
			cout << " -> Health: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << "hp" << endl;
			cout << " -> Speed:  " << p1.getSpeed() << endl;
			cout << "------------------------------------------" << endl;
			cout << e1.getName() << endl;
			cout << " -> Health: " << e1.getCurrentHp() << "/" << e1.getMaxHp() << "hp" << endl;
			cout << " -> Speed:  " << e1.getSpeed() << endl;
			cout << "==========================================" << endl;
			cout << "Your Options:" << endl << endl;
			for(int i=0;i<4;i++){
				if(p1.getSpells()[i] == nullptr) cout << " " << i+1 << ". Empty spell slot" << endl;
				else{
					cout << " " << i+1 << ". " << p1.getSpells()[i]->getName();
					if(p1.getSpells()[i]->getRemainingCooldown() == 0) cout << "   -> Status: Ready" << endl;  
					else if(p1.getSpells()[i]->getRemainingCooldown() == 1) cout << "   -> Status: 1 round until ready" << endl;  
					else cout << "   -> Status: " << p1.getSpells()[i]->getRemainingCooldown() << " rounds until ready" << endl;  
				}
			}
			cout << "------------------------------------------" << endl;
			cout << "What do you plan on doing adventurer? ";
			cin >> choice;
			cout << "==========================================" << endl;
			system("cls");
			
			while(choice > 4){
				choice = choice - 4;
			}
			
			if(p1.getSpeed() >= e1.getSpeed()){
				cout << "==========================================" << endl;
				b1.useSpellOnEnemy(choice-1);
				
				cout << endl;
				
				Spell* es = e1.getSpells()[randomiseSpellEnemy];
				if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
					b1.attackPlayer();
				} else {
					b1.useSpellOnPlayer(randomiseSpellEnemy);
				}
				
			} else {
				cout << "==========================================" << endl;
				Spell* es = e1.getSpells()[randomiseSpellEnemy];
				if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
					b1.attackPlayer();
				} else {
					b1.useSpellOnPlayer(randomiseSpellEnemy);
				}
				
				cout << endl;
				
				b1.useSpellOnEnemy(choice-1);
			}
			break;
		default:
			break;
		}
		
		if(b1.getIsFinished() == 1){
			
			//PLAYER PROHRAL, DODELAT DIALOGY
			cout;
		}
		
		b1.roundIncrement();
		b1.checkDeaths();
	}
	
	return 0;
}

