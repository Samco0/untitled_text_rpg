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
	
	Spell sp1("Shadow claw", "Claw of shadow", 10, 3);
	Spell sp2("Frostball", "A ball of frost lol", 4, 3);
	ChainingSpell sp3("Thunder barrage", "Barrage of thunders", 2.5, 5, 1, 6);
	Spell* s1[4] = { &sp1, &sp2, &sp3, nullptr };
	
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
		Battle b1(&p1, enemies[i]);
		while(b1.getIsFinished() == 0){
			randomiseAttackEnemy = rand() % 10;
			randomiseSpellEnemy = rand() % 4;
			
			cout << "==========================================" << endl;
			cout << p1.getName() << endl;
			cout << " -> Health: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << "hp" << endl;
			cout << " -> Level:  " << p1.getLevel() << endl;
			cout << " -> Speed:  " << p1.getSpeed() << endl;
			cout << "------------------------------------------" << endl;
			cout << enemies[i]->getName() << endl;
			cout << " -> Health: " << enemies[i]->getCurrentHp() << "/" << enemies[i]->getMaxHp() << "hp" << endl;
			cout << " -> Level:  " << enemies[i]->getLevel() << endl;
			cout << " -> Speed:  " << enemies[i]->getSpeed() << endl;
			cout << "==========================================" << endl;
			cout << "Your Options:" << endl;
			cout << " 1. Basic attack" << endl;
			cout << " 2. Use spell" << endl;
			cout << "------------------------------------------" << endl;
			cout << "Whats your choice, adventurer? ";
			cin >> choice;
			cout << "==========================================" << endl << endl;
			system("cls");
			
			switch (choice) {
			case 1:
				if(p1.getSpeed() >= enemies[i]->getSpeed()){
					cout << "==========================================" << endl;
					b1.attackEnemy();
					
					cout << endl;
					
					Spell* es = enemies[i]->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
						b1.attackPlayer();
					} else {
						b1.useSpellOnPlayer(randomiseSpellEnemy);
					}
					
				} else {
					cout << "==========================================" << endl;
					Spell* es = enemies[i]->getSpells()[randomiseSpellEnemy];
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
				ChainingSpell* cs;
				
				cout << "==========================================" << endl;
				cout << p1.getName() << endl;
				cout << " -> Health: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << "hp" << endl;
				cout << " -> Level:  " << enemies[i]->getLevel() << endl;
				cout << " -> Speed:  " << p1.getSpeed() << endl;
				cout << "------------------------------------------" << endl;
				cout << enemies[i]->getName() << endl;
				cout << " -> Health: " << enemies[i]->getCurrentHp() << "/" << enemies[i]->getMaxHp() << "hp" << endl;
				cout << " -> Level:  " << enemies[i]->getLevel() << endl;
				cout << " -> Speed:  " << enemies[i]->getSpeed() << endl;
				cout << "==========================================" << endl;
				cout << "Your spells:" << endl << endl;
				for(int i=0;i<4;i++){
					if(p1.getSpells()[i] == nullptr) cout << " " << i+1 << ". Empty spell slot" << endl;
					else{
						cs = dynamic_cast<ChainingSpell*>(p1.getSpells()[i]);
						
						cout << " " << i+1 << ". " << p1.getSpells()[i]->getName();
						if(p1.getSpells()[i]->getRemainingCooldown() == 0) cout << endl << "   -> Status: Ready" << endl;  
						else if(p1.getSpells()[i]->getRemainingCooldown() == 1) cout << endl << "   -> Status: 1 round until ready" << endl;  
						else cout << endl << "   -> Status: " << p1.getSpells()[i]->getRemainingCooldown() << " rounds until ready" << endl;  
						
						if(cs != nullptr){
							cout << "   -> Type of spell: Chaining attack spell" << endl;
							cout << "   -> Damage per hit: " << cs->getDmg() << " damage" << endl;  
							cout << "   -> Minimal hits: " << cs->getMinHits() << " hits" << endl;
							cout << "   -> Maximal hits: " << cs->getMaxHits() << " hits" << endl;
						}
						else{
							cout << "   -> Type of spell: Basic attack spell" << endl;
							cout << "   -> Damage: " << p1.getSpells()[i]->getDmg() << endl;  
						}
						cout << "   -> Description: " << p1.getSpells()[i]->getDescription() << endl << endl;
					}
				}
				cout << "------------------------------------------" << endl;
				cout << "Whats your choice, adventurer? ";
				cin >> choice;
				cout << "==========================================" << endl;
				system("cls");
				
				while(choice > 4){
					choice = choice - 4;
				}
				
				if(p1.getSpeed() >= enemies[i]->getSpeed()){
					cout << "==========================================" << endl;
					b1.useSpellOnEnemy(choice-1);
					
					cout << endl;
					
					Spell* es = enemies[i]->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0){
						b1.attackPlayer();
					} else {
						b1.useSpellOnPlayer(randomiseSpellEnemy);
					}
					
				} else {
					cout << "==========================================" << endl;
					Spell* es = enemies[i]->getSpells()[randomiseSpellEnemy];
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
			
			b1.roundIncrement();
			b1.checkDeaths();
		}	
	}
	
	return 0;
}

