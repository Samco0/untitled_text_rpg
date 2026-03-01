#include "battleManager.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//constructors
BattleManager::BattleManager(){
	this->round = 0;
	this->isFinished = 0;
	this->player = nullptr;
	this->enemy = nullptr;
}
BattleManager::BattleManager(CombatCharacter* player, CombatCharacter* enemy){
	this->round = 0;
	this->isFinished = 0;
	this->player = player;
	this->enemy = enemy;
}

//getters
int BattleManager::getRound(){return this->round;}
int BattleManager::getIsFinished(){return this->isFinished;}
CombatCharacter* BattleManager::getPlayer(){return this->player;}
CombatCharacter* BattleManager::getEnemy(){return this->enemy;}

//setters
void BattleManager::setRound(int round){this->round = round;}
void BattleManager::setIsFinished(int isFinished){this->isFinished = isFinished;}
void BattleManager::setPlayer(CombatCharacter* player){this->player = player;}
void BattleManager::setEnemy(CombatCharacter* enemy){this->enemy = enemy;}

//battle methods
void BattleManager::attack(CombatCharacter* attacker, CombatCharacter* target){
	if(attacker->getCurrentHp() <= 0) return;
	
	int crit = rand() % 100 + 1;
	
	Player* pA = dynamic_cast<Player*>(attacker);
	float damage = attacker->getDmg();
	if(pA != nullptr){
		damage += pA->getWeapon()->getDmg();
	}
	
	bool isCrit = false;
	
	if(crit <= attacker->getCritChance()){
		damage *= attacker->getCritValue();
		isCrit = true;
	}
	
	target->setCurrentHp(target->getCurrentHp() - damage);
	
	if(pA != nullptr){
		cout << " -> You (" << attacker->getName() << ") dealt " << damage << " damage to " << target->getName() << " using " << pA->getWeapon()->getName();
		
		if(isCrit) cout << ". It was a critical hit.";
		cout << endl;
	}
	else{
		Player* pT = dynamic_cast<Player*>(target);
		
		cout << " -> " << attacker->getName() << " dealt " << damage << " damage to you (" << pT->getName() << ")";
		
		if(isCrit) cout << ". It was a critical hit.";
		cout << endl;
	}
}

void BattleManager::attackUsingSpell(CombatCharacter* attacker, CombatCharacter* target, int spellIndex){
	if(attacker->getCurrentHp() <= 0) return;
	
	bool attackerIsPlayer = (dynamic_cast<Player*>(attacker) != nullptr);
	
	Spell* s = attacker->getSpells()[spellIndex];
	
	//spell is invalid
	if(s == nullptr){
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName() << ") tried to cast a spell." << endl;
			cout << " -> But didn't notice, that you can't cast a nonexistent spell." << endl;
			cout << " -> You attacked using your normal attack." << endl;
		}
		else{
			cout << " -> " << attacker->getName() << " tried to cast a spell." << endl;
			cout << " -> But there is no spell in that slot." << endl;
			cout << " -> " << attacker->getName() << " used a basic attack instead." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	//cooldown
	if(s->getRemainingCooldown() != 0){
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName()
			<< ") tried to cast a spell, " << s->getName() << "." << endl;
			cout << " -> But the spell didn't come out, it's not ready yet." << endl;
			cout << " -> You attacked using your normal attack." << endl;
		}
		else{
			cout << " -> " << attacker->getName()
			<< " tried to cast " << s->getName() << "." << endl;
			cout << " -> But the spell is still on cooldown." << endl;
			cout << " -> " << attacker->getName()
			<< " used a basic attack instead." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	cout << " -> " << (attackerIsPlayer ? "You (" : "") << attacker->getName()<< (attackerIsPlayer ? ")" : "") << " casted a spell, " << s->getName() << "." << endl;
	
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	StatusEffectSpell* ses = dynamic_cast<StatusEffectSpell*>(s);
	
	//chaining spell
	if(cs != nullptr){
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1)
		+ cs->getMinHits();
		
		float totalDmg = hits * cs->getDmg();
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		
		cout << " -> Spell hit " << hits << " times!" << endl;
		
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName()
			<< ") dealt " << totalDmg
			<< " damage to " << target->getName() << "." << endl;
		}
		else{
			cout << " -> " << attacker->getName()
			<< " dealt " << totalDmg
			<< " damage to you (" << target->getName() << ")." << endl;
		}
	}
	
	//status effect spell
	else if(ses != nullptr){
		target->setCurrentHp(target->getCurrentHp() - s->getDmg());
		
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName()
			<< ") dealt " << s->getDmg()
			<< " damage to " << target->getName() << "." << endl;
		}
		else{
			cout << " -> " << attacker->getName()
			<< " dealt " << s->getDmg()
			<< " damage to you (" << target->getName() << ")." << endl;
		}
		
		if(rand() % 100 + 1 <= ses->getChanceToRecieve() && ses->getStatusToGive() != nullptr){
			cout << " -> " << target->getName()
			<< " received the "
			<< ses->getStatusToGive()->getName()
			<< " status effect." << endl;
			
			target->addStatusEffect(ses->getStatusToGive());
		}
	}
	
	//normal spell
	else{
		target->setCurrentHp(target->getCurrentHp() - s->getDmg());
		
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName()
			<< ") dealt " << s->getDmg()
			<< " damage to " << target->getName() << "." << endl;
		}
		else{
			cout << " -> " << attacker->getName()
			<< " dealt " << s->getDmg()
			<< " damage to you (" << target->getName() << ")." << endl;
		}
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}


//battle misc
void BattleManager::roundIncrement(){
	this->round++;
	
	for(int i=0;i<4;i++){
		if(this->player->getSpells()[i] != nullptr){
			if(this->player->getSpells()[i]->getRemainingCooldown() != 0){
				this->player->getSpells()[i]->setRemainingCooldown(
					this->player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
		
		if(this->enemy->getSpells()[i] != nullptr){
			if(this->enemy->getSpells()[i]->getRemainingCooldown() != 0){
				this->enemy->getSpells()[i]->setRemainingCooldown(
					this->enemy->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
	}
}

void BattleManager::checkDeaths(){
	if(this->player->getCurrentHp() <= 0){
		this->player->setCurrentHp(0);
		this->isFinished = 1;
		
		cout << "==========================================" << endl;
		cout << " -> You (" << this->player->getName() << ") lost the battle." << endl;
		cout << "==========================================" << endl;
	}
	else if(this->enemy->getCurrentHp() <= 0){
		this->enemy->setCurrentHp(0);
		this->isFinished = 2;
		
		cout << "==========================================" << endl;
		cout << " -> You (" << this->player->getName() << ") won the battle." << endl;
		cout << "==========================================" << endl;
		
		Player* p = dynamic_cast<Player*>(this->player);
		Enemy* e = dynamic_cast<Enemy*>(this->enemy);
		
		p->setCurrentXp(p->getCurrentXp() + e->getXpToGet());
	}
	else{
		this->isFinished = 0;
	}
}

void BattleManager::checkStatusEffects(){
	HpStatusEffect* hps;
	
	bool playerFirst = this->player->getSpeed() >= this->enemy->getSpeed();
	
	for(int i=0;i<4;i++){
		CombatCharacter* first  = playerFirst ? (CombatCharacter*)player : (CombatCharacter*)enemy;
		CombatCharacter* second = playerFirst ? (CombatCharacter*)enemy : (CombatCharacter*)player;
		
		for(int turn=0; turn<2; turn++){
			CombatCharacter* current = (turn==0 ? first : second);
			
			StatusEffect* se = current->getStatusEffect()[i];
			if(se == nullptr) continue;
			
			hps = dynamic_cast<HpStatusEffect*>(se);
			if(hps == nullptr) continue;
			
			current->setCurrentHp(current->getCurrentHp() + hps->getHpAffection());
			
			cout << "==========================================" << endl;
			
			bool isPlayer = (current == player);
			
			if(hps->getHpAffection() < 0.0){
				if(isPlayer) cout << " -> You (" << player->getName() << ") got hurt by the " << hps->getName() << " status effect. It did " << hps->getHpAffection()*(-1) << " damage." << endl;
				else cout << " -> " << enemy->getName() << " got hurt by the " << hps->getName() << " status effect. It did " << hps->getHpAffection()*(-1) << " damage." << endl;
			}
			else {
				if(isPlayer) cout << " -> You (" << player->getName() << ") got healed by the " << hps->getName() << " status effect. It recovered " << hps->getHpAffection() << " hp." << endl;
				else cout << " -> " << enemy->getName() << " got healed by the " << hps->getName() << " status effect. It recovered " << hps->getHpAffection() << " hp." << endl;
			}
		}
	}
}

//the battle itself
void BattleManager::battle() {
	int choice;
	srand(time(0));
	
	while(this->isFinished == 0) {
		int randomiseAttackEnemy = rand() % 10;
		int randomiseSpellEnemy = rand() % 4;
		
		cout << "==========================================" << endl;
		cout << player->getName() << endl;
		cout << " -> Health: " << player->getCurrentHp() << "/" << player->getMaxHp() << "hp" << endl;
		cout << " -> Level: " << player->getLevel() << endl;
		cout << " -> Speed: " << player->getSpeed() << endl;
		cout << "------------------------------------------" << endl;
		cout << enemy->getName() << endl;
		cout << " -> Health: " << enemy->getCurrentHp() << "/" << enemy->getMaxHp() << "hp" << endl;
		cout << " -> Level: " << enemy->getLevel() << endl;
		cout << " -> Speed: " << enemy->getSpeed() << endl;
		cout << "==========================================" << endl;
		
		cout << "Your Options:" << endl;
		cout << " 1. Basic attack" << endl;
		cout << " 2. Use spell" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Whats your choice, adventurer? ";
		cin >> choice;
		cout << "==========================================" << endl << endl;
		system("cls");
		
		switch(choice) {
			case 1: {
				cout << "==========================================" << endl;
				if(player->getSpeed() >= enemy->getSpeed()) {
					attack(player, enemy);
					cout << endl;
					
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0) {
						attack(enemy, player);
					} else {
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
					}
				} else {
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0) {
						attack(enemy, player);
					} else {
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
					}
					
					cout << endl;
					attack(player, enemy);
				}
				break;
			}
			
			case 2: {
				ChainingSpell* cs;
				StatusEffectSpell* sas;
				
				cout << "==========================================" << endl;
				cout << player->getName() << endl;
				cout << " -> Health: " << player->getCurrentHp() << "/" << player->getMaxHp() << "hp" << endl;
				cout << " -> Level: " << player->getLevel() << endl;
				cout << " -> Speed: " << player->getSpeed() << endl;
				cout << "------------------------------------------" << endl;
				cout << enemy->getName() << endl;
				cout << " -> Health: " << enemy->getCurrentHp() << "/" << enemy->getMaxHp() << "hp" << endl;
				cout << " -> Level: " << enemy->getLevel() << endl;
				cout << " -> Speed: " << enemy->getSpeed() << endl;
				cout << "==========================================" << endl;
				
				cout << "Your spells:" << endl << endl;
				for(int i = 0; i < 4; i++) {
					if(player->getSpells()[i] == nullptr) {
						cout << " " << i+1 << ". Empty spell slot" << endl;
					} else {
						cs = dynamic_cast<ChainingSpell*>(player->getSpells()[i]);
						sas = dynamic_cast<StatusEffectSpell*>(player->getSpells()[i]);
						cout << " " << i+1 << ". " << player->getSpells()[i]->getName();
						
						if(player->getSpells()[i]->getRemainingCooldown() == 0)
							cout << endl << " -> Status: Ready" << endl;
						else if(player->getSpells()[i]->getRemainingCooldown() == 1)
							cout << endl << " -> Status: 1 round until ready" << endl;
						else
							cout << endl << " -> Status: " << player->getSpells()[i]->getRemainingCooldown() << " rounds until ready" << endl;
						
						if(cs != nullptr) {
							cout << " -> Type of spell: Chaining attack spell" << endl;
							cout << " -> Damage per hit: " << cs->getDmg() << " damage" << endl;
							cout << " -> Minimal hits: " << cs->getMinHits() << " hits" << endl;
							cout << " -> Maximal hits: " << cs->getMaxHits() << " hits" << endl;
						} else if(sas != nullptr) {
							cout << " -> Type of spell: Status effect spell" << endl;
							cout << " -> Damage: " << sas->getDmg() << endl;
							cout << " -> Status effect: " << sas->getStatusToGive()->getName() << endl;
						} else {
							cout << " -> Type of spell: Basic attack spell" << endl;
							cout << " -> Damage: " << player->getSpells()[i]->getDmg() << endl;
						}
						
						cout << " -> Description: " << player->getSpells()[i]->getDescription() << endl << endl;
					}
				}
				
				cout << "------------------------------------------" << endl;
				cout << "Whats your choice, adventurer? ";
				cin >> choice;
				cout << "==========================================" << endl;
				system("cls");
				
				while(choice > 4) choice -= 4;
				
				cout << "==========================================" << endl;
				if(player->getSpeed() >= enemy->getSpeed()) {
					attackUsingSpell(player, enemy, choice-1);
					cout << endl;
					
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
						attack(enemy, player);
					else
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
				} else {
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					if(randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
						attack(enemy, player);
					else
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
					
					cout << endl;
					attackUsingSpell(player, enemy, choice-1);
				}
				
				break;
			}
			
			default: break;
		}
		
		roundIncrement();
		checkStatusEffects();
		checkDeaths();
	}
}

