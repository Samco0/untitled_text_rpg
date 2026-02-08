#include "battle.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//constructors
Battle::Battle(){
	this->round = 0;
	this->isFinished = 0;
	this->player = nullptr;
	this->enemy = nullptr;
}
Battle::Battle(Player* player, Enemy* enemy){
	this->round = 0;
	this->isFinished = 0;
	this->player = player;
	this->enemy = enemy;
}

//getters
int Battle::getRound(){return this->round;}
int Battle::getIsFinished(){return this->isFinished;}
Player* Battle::getPlayer(){return this->player;}
Enemy* Battle::getEnemy(){return this->enemy;}

//setters
void Battle::setRound(int round){this->round = round;}
void Battle::setIsFinished(int isFinished){this->isFinished = isFinished;}
void Battle::setPlayer(Player* player){this->player = player;}
void Battle::setEnemy(Enemy* enemy){this->enemy = enemy;}

//battle methods
void Battle::attackEnemy(){
	int crit = rand() % 100 + 1;
	
	float damage = player->getDmg();
	if(crit <= player->getCritChance()){
		damage *= player->getCritValue();
		cout << "You (" << player->getName() << ") dealt " << damage << " damage to " << enemy->getName() << ". It was a critical hit." << endl << endl;
	} else {
		cout << "You (" << player->getName() << ") dealt " << damage << " damage to " << enemy->getName() << "." << endl << endl;
	}
	enemy->setCurrentHp(enemy->getCurrentHp() - damage);
}

void Battle::attackPlayer(){
	int crit = rand() % 100 + 1;
	
	float damage = enemy->getDmg();
	if(crit <= enemy->getCritChance()){
		damage *= enemy->getCritValue();
		cout << enemy->getName() << " dealt " << damage << " damage to you (" << player->getName() << "). It was a critical hit." << endl << endl;
	} else {
		cout << enemy->getName() << " dealt " << damage << " damage to you (" << player->getName() << ")." << endl << endl;
	}
	player->setCurrentHp(player->getCurrentHp() - damage);
}

void Battle::useSpellOnEnemy(int spellIndex){
	Spell* s = player->getSpells()[spellIndex];
	
	if(s == nullptr){
		cout << "No spell in this slot!" << endl;
		return;
	}
	
	// here we are finding out if our spell is chaining
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	
	cout << "You (" << player->getName() << ") casted a spell, " << s->getName() << "." << endl;
	
	if(cs != nullptr){
		// chaining spell
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
		
		float totalDmg = hits * cs->getDmg();
		
		enemy->setCurrentHp(enemy->getCurrentHp() - totalDmg);
		
		cout << "Spell hit " << hits << " times!" << endl;
		cout << "You (" << player->getName() << ") dealt " << totalDmg << " damage to " << enemy->getName() << "." << endl << endl;
	}
	else{
		// average spell
		enemy->setCurrentHp(enemy->getCurrentHp() - s->getDmg());
		cout << "You (" << player->getName() << ") dealt " << s->getDmg() << " damage to " << enemy->getName() << "." << endl << endl;
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}


void Battle::useSpellOnPlayer(int spellIndex){
	Spell* s = enemy->getSpells()[spellIndex];
	
	if(s == nullptr){
		cout << "No spell in this slot!" << endl;
		return;
	}
	
	// here we are finding out if our spell is chaining
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	
	cout << enemy->getName() << " casted a spell, " << s->getName() << "." << endl;
	
	if(cs != nullptr){
		// chaining spell
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
		
		float totalDmg = hits * cs->getDmg();
		
		player->setCurrentHp(player->getCurrentHp() - totalDmg);
		
		cout << "Spell hit " << hits << " times!" << endl;
		cout << enemy->getName() << " dealt " << totalDmg << " damage to you (" << player->getName() << ")." << endl << endl;
	}
	else{
		// average spell
		enemy->setCurrentHp(enemy->getCurrentHp() - s->getDmg());
		cout << enemy->getName() << " dealt " << s->getDmg() << " damage to you (" << player->getName() << ")." << endl << endl;
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}

//battle misc
void Battle::roundIncrement(){
	this->round++;
	for(int i=0;i<4;i++){
		if(player->getSpells()[i] != nullptr){
			if(player->getSpells()[i]->getRemainingCooldown() != 0){
				player->getSpells()[i]->setRemainingCooldown(player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
		if(enemy->getSpells()[i] != nullptr){
			if(enemy->getSpells()[i]->getRemainingCooldown() != 0){
				enemy->getSpells()[i]->setRemainingCooldown(player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
	}
}
void Battle::checkDeaths(){
	if(player->getCurrentHp() <= 0){
		player->setCurrentHp(0);
		if(round == 1) cout << "You (" << player->getName() << ") lost the battle in 1 round. Wow." << endl;
		else cout << "You (" << player->getName() << ") lost the battle in " << round << " rounds." << endl;
		isFinished = 1;
	}
	else if(enemy->getCurrentHp() <= 0){
		enemy->setCurrentHp(0);
		if(round == 1) cout << "You (" << player->getName() << ") won the battle in 1 round. Amazing work!" << endl;
		else cout << "You (" << player->getName() << ") won the battle in " << round << " rounds." << endl;
		isFinished = 2;
	}
	else {
		isFinished = 0;
	}
}

