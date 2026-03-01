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
		string weaponName = pA->getWeapon()->getName();
		string targetName = target->getName();
		
		if(pA->getWeapon()->getType() == "Sword"){
			cout << " -> You (" << attacker->getName() << ") slash with " << weaponName << ", rending through " << targetName << "'s defenses for " << damage << " damage";
			if(isCrit) cout << ". The strike cleaves bone and spirit!" ;
		}
		else{
			cout << " -> You (" << attacker->getName() << ") attack with " << weaponName << ", dealing " << damage << " damage to " << targetName;
			if(isCrit) cout << ". A vicious strike!" ;
		}
		
		cout << endl;
	}
	else{
		Player* pT = dynamic_cast<Player*>(target);
		
		cout << " -> " << attacker->getName() << " lashes out at you (" << pT->getName() << "), rending " << damage << " damage";
		if(isCrit) cout << ". A critical strike!" ;
		
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
			cout << " -> You (" << attacker->getName() << ") reached into the void to summon a spell." << endl;
			cout << " -> But the abyss answered with silence. No such power exists." << endl;
			cout << " -> Steel it is, then. You strike with a mortal blow." << endl;
		}
		else{
			cout << " -> " << attacker->getName() << " whispered to unseen forces." << endl;
			cout << " -> Nothing answered. The spell was but an empty promise." << endl;
			cout << " -> " << attacker->getName() << " lunged forward with a crude, physical strike." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	//cooldown
	if(s->getRemainingCooldown() != 0){
		if(attackerIsPlayer){
			cout << " -> You (" << attacker->getName() << ") invoked the forbidden name of " << s->getName() << "." << endl;
			cout << " -> Yet its power still slumbers, bound by unseen chains." << endl;
			cout << " -> Frustrated, you carve your will into flesh with a basic attack." << endl;
		}
		else{
			cout << " -> " << attacker->getName() << " attempted to unleash " << s->getName() << "." << endl;
			cout << " -> But the spell flickered and died — its essence not yet restored." << endl;
			cout << " -> With a snarl, " << attacker->getName() << " resorts to a savage physical assault." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	cout << " -> " << (attackerIsPlayer ? "You (" : "") << attacker->getName() << (attackerIsPlayer ? ")" : "") << " muttered the cursed words of " << s->getName()  << ", and shadows writhed around your hands." << endl;
	
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	StatusEffectSpell* ses = dynamic_cast<StatusEffectSpell*>(s);
	
	//chaining spell
	if(cs != nullptr){
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1)
		+ cs->getMinHits();
		
		float totalDmg = hits * cs->getDmg();
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		
		cout << " -> Spell hit " << hits << " times!" << endl;
		
		cout << " -> The spell lashes out " << hits << " times, shadows and fire converging!" << endl;
		
		if(attackerIsPlayer) cout << " -> You (" << attacker->getName() << ") sear " << target->getName() << " for " << totalDmg << " damage." << endl;
		else	cout << " -> " << attacker->getName() << "'s spell scorches you, inflicting " << totalDmg << " damage!" << endl;
	}
	
	//status effect spell
	else if(ses != nullptr){
		target->setCurrentHp(target->getCurrentHp() - s->getDmg());
		
		if(attackerIsPlayer){
			cout << " -> Your " << s->getName() << " wracks " << target->getName() << " for " << s->getDmg() << " damage." << endl;
		} else {
			cout << " -> " << s->getName() << " tears into you (" << target->getName() << "), dealing " << s->getDmg() << " damage." << endl;
		}
		
		if(rand() % 100 + 1 <= ses->getChanceToRecieve() && ses->getStatusToGive() != nullptr){
			cout << " -> " << target->getName() << " is cursed with " << ses->getStatusToGive()->getName() << ", a lingering shadow over their vitality." << endl;
			
			target->addStatusEffect(ses->getStatusToGive());
		}
	}
	
	//normal spell
	else{
		target->setCurrentHp(target->getCurrentHp() - s->getDmg());
		
		if(attackerIsPlayer) cout << " -> You (" << attacker->getName() << ") unleash the arcane fury of " << s->getName() << ", rending " << target->getName()	<< " for " << s->getDmg() << " damage!" << endl;
		else cout << " -> " << attacker->getName() << " strikes you with " << s->getName() << ", searing flesh and spirit for " << s->getDmg() << " damage!" << endl;
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}


//battle misc
void BattleManager::roundIncrement(){
	this->round++;
	
	for(int i=0;i<4;i++){
		if(this->player->getSpells()[i] != nullptr){
			if(this->player->getSpells()[i]->getRemainingCooldown() != 0){
				this->player->getSpells()[i]->setRemainingCooldown(this->player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
		
		if(this->enemy->getSpells()[i] != nullptr){
			if(this->enemy->getSpells()[i]->getRemainingCooldown() != 0){
				this->enemy->getSpells()[i]->setRemainingCooldown(this->enemy->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
	}
}

void BattleManager::checkDeaths(){
	if(this->player->getCurrentHp() <= 0){
		this->player->setCurrentHp(0);
		this->isFinished = 1;
		
		cout << "==========================================" << endl;
		cout << " -> You (" << this->player->getName() << ") have fallen… the shadows claim you." << endl;
		cout << " -> Your journey ends here, at the edge of blood and ruin." << endl;
		cout << "==========================================" << endl;
		
		system("pause");
	}
	else if(this->enemy->getCurrentHp() <= 0){
		this->enemy->setCurrentHp(0);
		this->isFinished = 2;
		
		cout << "==========================================" << endl;
		cout << " -> Victory! You (" << this->player->getName() << ") stand over the vanquished " << this->enemy->getName() << "." << endl;
		cout << " -> Their life force fuels your growth." << endl;
		cout << "==========================================" << endl;
		
		Player* p = dynamic_cast<Player*>(this->player);
		Enemy* e = dynamic_cast<Enemy*>(this->enemy);
		
		float xpGained = e->getXpToGet();
		p->setCurrentXp(p->getCurrentXp() + xpGained);
		cout << " -> You gained " << xpGained << " XP from this battle." << endl;
		
		system("pause");
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
				if(isPlayer) 
					cout << " -> Dark forces from " << hps->getName() << " bite you, dealing " << -hps->getHpAffection() << " damage." << endl;
				else 
					cout << " -> " << enemy->getName() << " suffers from " << hps->getName() << ", losing " << -hps->getHpAffection() << " hp." << endl;
			} else {
				if(isPlayer)
					cout << " -> A gentle light of " << hps->getName() << " heals you for " << hps->getHpAffection() << " hp." << endl;
				else
					cout << " -> " << enemy->getName() << " is restored by " << hps->getName() << " for " << hps->getHpAffection() << " hp." << endl;
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
		cout << *player;
		cout << "------------------------------------------" << endl;
		cout << *enemy;
		cout << "==========================================" << endl;
		
		cout << "Your Options:" << endl;
		cout << " 1. Strike with weapon" << endl;
		cout << " 2. Invoke a spell" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Choose your fate, adventurer: ";
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
				Spell* s;
				ChainingSpell* cs;
				StatusEffectSpell* sas;
				
				cout << "==========================================" << endl;
				cout << *player;
				cout << "------------------------------------------" << endl;
				cout << *enemy;
				cout << "==========================================" << endl;
				
				cout << "Your spells, etched into your soul:" << endl << endl;
				for(int i = 0; i < 4; i++) {
					Spell *s = player->getSpells()[i];
					
					cout << " " << i+1 << ". ";
					cout << "Etched into your soul: ";
					if(s == nullptr) { 
						cout << "A hollow space within your mind." << endl << endl;
					} else {
						ChainingSpell* cs = dynamic_cast<ChainingSpell*>(player->getSpells()[i]);
						StatusEffectSpell* sas = dynamic_cast<StatusEffectSpell*>(player->getSpells()[i]);
						
						if(cs != nullptr) {
							cout << *cs;
						} else if(sas != nullptr) {
							cout << *sas;
						} else {
							cout << *s;
						}
					}
				}
				
				cout << "------------------------------------------" << endl;
				cout << "Which spell shall you unleash, adventurer? ";
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
