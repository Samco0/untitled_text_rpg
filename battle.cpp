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
	
	if(player->getCurrentHp() > 0){
		float damage = player->getDmg();
		if(crit <= player->getCritChance()){
			damage *= player->getCritValue();
			cout << " -> " << "You (" << player->getName() << ") dealt " << damage << " damage to " << enemy->getName() << ". It was a critical hit." << endl;
		} else {
			cout << " -> " << "You (" << player->getName() << ") dealt " << damage << " damage to " << enemy->getName() << "." << endl;
		}
		enemy->setCurrentHp(enemy->getCurrentHp() - damage);
	}
}

void Battle::attackPlayer(){
	int crit = rand() % 100 + 1;
	
	if(enemy->getCurrentHp() > 0){
		float damage = enemy->getDmg();
		if(crit <= enemy->getCritChance()){
			damage *= enemy->getCritValue();
			cout << " -> " << enemy->getName() << " dealt " << damage << " damage to you (" << player->getName() << "). It was a critical hit." << endl;
		} else {
			cout << " -> " << enemy->getName() << " dealt " << damage << " damage to you (" << player->getName() << ")." << endl;
		}
		player->setCurrentHp(player->getCurrentHp() - damage);
	}
}

void Battle::useSpellOnEnemy(int spellIndex){
	Spell* s = player->getSpells()[spellIndex];
	
	if(s == nullptr){
		cout << " -> " << "You (" << player->getName() << ") tried to cast a spell." << endl;
		cout << " -> " << "But didn't notice, that you can't cast a nonexistent spell." << endl;
		cout << " -> " << "You attacked using your normal attack." << endl;
		
		attackEnemy();
	}
	else if(s->getRemainingCooldown() != 0){
		cout << " -> " << "You (" << player->getName() << ") tried to cast a spell, " << s->getName() << "." << endl;
		cout << " -> " << "But the spell didn't come out, it's not ready yet." << endl;
		cout << " -> " << "You attacked using your normal attack." << endl;
		
		attackEnemy();
	}
	else if(player->getCurrentHp() > 0 && s->getRemainingCooldown() == 0){
		// here we are finding out if our spell is chaining
		ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
		
		cout << " -> " << "You (" << player->getName() << ") casted a spell, " << s->getName() << "." << endl;
		
		if(cs != nullptr){
			// chaining spell
			int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
			
			float totalDmg = hits * cs->getDmg();
			
			enemy->setCurrentHp(enemy->getCurrentHp() - totalDmg);
			
			cout << " -> " << "Spell hit " << hits << " times!" << endl;
			cout << " -> " << "You (" << player->getName() << ") dealt " << totalDmg << " damage to " << enemy->getName() << "." << endl;
		}
		else{
			// average spell
			enemy->setCurrentHp(enemy->getCurrentHp() - s->getDmg());
			cout << " -> " << "You (" << player->getName() << ") dealt " << s->getDmg() << " damage to " << enemy->getName() << "." << endl;
		}
		
		s->setRemainingCooldown(s->getFullCooldown());
	}
}


void Battle::useSpellOnPlayer(int spellIndex){
	Spell* s = enemy->getSpells()[spellIndex];
	
	if(s == nullptr){
		cout << " -> " << enemy->getName() << " tried to cast a spell." << endl;
		cout << " -> " << "But there is no spell in that slot." << endl;
		cout << " -> " << enemy->getName() << " used a basic attack instead." << endl;
		
		attackPlayer();
	}
	else if(s->getRemainingCooldown() != 0){
		cout << " -> " << enemy->getName() << " tried to cast " << s->getName() << "." << endl;
		cout << " -> " << "But the spell is still on cooldown." << endl;
		cout << " -> " << enemy->getName() << " used a basic attack instead." << endl;
		
		attackPlayer();
	}
	else if(enemy->getCurrentHp() > 0 && s->getRemainingCooldown() == 0){
		// here we are finding out if our spell is chaining
		ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
		
		cout << " -> " << enemy->getName() << " casted a spell, " << s->getName() << "." << endl;
		
		if(cs != nullptr){
			// chaining spell
			int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
			
			float totalDmg = hits * cs->getDmg();
			
			player->setCurrentHp(player->getCurrentHp() - totalDmg);
			
			cout << " -> Spell hit " << hits << " times!" << endl;
			cout << " -> " << enemy->getName() << " dealt " << totalDmg << " damage to you (" << player->getName() << ")." << endl;
		}
		else{
			// average spell
			player->setCurrentHp(player->getCurrentHp() - s->getDmg());
			
			cout << " -> " << enemy->getName() << " dealt " << s->getDmg() << " damage to you (" << player->getName() << ")." << endl;
		}
		
		s->setRemainingCooldown(s->getFullCooldown());
	}
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
		player->setCurrentHp(0); //player lost
		isFinished = 1;
		cout << endl << " -> You (" << player->getName() << ") lost the battle." << endl;
		cout << "==========================================" << endl;
	}
	else if(enemy->getCurrentHp() <= 0){
		enemy->setCurrentHp(0); //player won
		isFinished = 2;
		cout << endl << " -> You (" << player->getName() << "} won the battle." << endl;
		cout << "==========================================" << endl;
		
		player->setCurrentXp(player->getCurrentXp() + enemy->getXpToGet());
	}
	else {
		isFinished = 0; //battle continues
	}
}

