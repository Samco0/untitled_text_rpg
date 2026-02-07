#include "battle.h"

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
	
	if(crit <= player->getCritChance()){
		this->enemy->setCurrentHp(enemy->getCurrentHp() - (player->getDmg()*player->getCritValue()));
		cout << "You (" << player->getName() << ") dealt " << (player->getDmg()*player->getCritValue()) << " damage to " << enemy->getName() << ". It was a critical hit." << endl << endl;
	}
	else{
		this->enemy->setCurrentHp(enemy->getCurrentHp() - player->getDmg());
		cout << "You (" << player->getName() << ") dealt " << player->getDmg() << " damage to " << enemy->getName() << "." << endl << endl;
	}
}
void Battle::attackPlayer(){
	int crit = rand() % 100 + 1;
	if(crit <= enemy->getCritChance() % 100 + 1){
		this->player->setCurrentHp(player->getCurrentHp() - (enemy->getDmg()*enemy->getCritValue()));
		cout << enemy->getName() << " dealth " << (enemy->getDmg()*enemy->getCritValue()) << " damage to you (" << player->getName() << "). It was a critical hit." << endl << endl;
	}
	else{
		this->player->setCurrentHp(player->getCurrentHp() - enemy->getDmg());
		cout << enemy->getName() << " dealt " << enemy->getDmg() << " damage to you (" << player->getName() << ")." << endl << endl;
	}
}
void Battle::roundIncrement(){this->round++;}
void Battle::checkDeaths(){
	if(this->player->getCurrentHp() == 0){
		if(this->round == 1) cout << "You (" << player->getName() << ") lost the battle in 1 round. Wow." << endl;
		else cout << "You (" << player->getName() << ") lost the battle in" << this->round << " rounds." << endl;
		this->isFinished = 1;
	}
	else if(this->enemy->getCurrentHp() == 0){
		if(this->round == 1) cout << "You (" << player->getName() << ") won the battle in 1 round. Amazing work!" << endl;
		else cout << "You (" << player->getName() << ") won the battle in " << this->round << " rounds." << endl;
		this->isFinished = 2;
	}
	else this->isFinished = 0;
}
