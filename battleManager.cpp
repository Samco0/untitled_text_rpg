#include "battleManager.h"
#include <iostream>
#include <cstdlib>
using namespace std;

//constructors
BattleManager::BattleManager() {
	this->round = 0;
	this->isFinished = 0;
	this->player = nullptr;
	this->enemy = nullptr;
}
BattleManager::BattleManager(CombatCharacter* player, CombatCharacter* enemy) {
	this->round = 0;
	this->isFinished = 0;
	this->player = player;
	this->enemy = enemy;
}

//getters
int BattleManager::getRound() {return this->round;}
int BattleManager::getIsFinished() {return this->isFinished;}
CombatCharacter* BattleManager::getPlayer() {return this->player;}
CombatCharacter* BattleManager::getEnemy() {return this->enemy;}

//setters
void BattleManager::setRound(int round) {this->round = round;}
void BattleManager::setIsFinished(int isFinished) {this->isFinished = isFinished;}
void BattleManager::setPlayer(CombatCharacter* player) {this->player = player;}
void BattleManager::setEnemy(CombatCharacter* enemy) {this->enemy = enemy;}

//battle methods
void BattleManager::attack(CombatCharacter* attacker, CombatCharacter* target) {
	if (attacker->getCurrentHp() <= 0) return;
	
	int crit = rand() % 100 + 1;
	
	//if the attacker is a player, he typically wields a weapon, here is the counting of it basically
	Player* pA = dynamic_cast<Player*>(attacker);
	float damage = attacker->getDmg();
	if (pA != nullptr) {
		damage += pA->getWeapon()->getDmg();
	}
	
	bool isCrit = false;
	
	//this one is for both attackers, its just multiplying the damage by value of critical hit and setting a bool to true
	if (crit <= attacker->getCritChance()) {
		damage *= attacker->getCritValue();
		isCrit = true;
	}
	
	
	target->setCurrentHp(target->getCurrentHp() - damage);
	
	if (pA != nullptr) {
		string weaponName = pA->getWeapon()->getName();
		string targetName = target->getName();
		
		string weaponType = pA->getWeapon()->getType();
		
		if (weaponType == "Sword") {
			cout << " -> You (" << attacker->getName() << ") swing " << weaponName << " in a deadly arc at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The strike cleaves bone and shadow alike!";
		} else if (weaponType == "Mace") {
			cout << " -> You (" << attacker->getName() << ") slam " << weaponName << " down upon " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The impact rattles armor and cracks sinew!";
		} else if (weaponType == "Whip") {
			cout << " -> You (" << attacker->getName() << ") lash " << weaponName << " with serpentine precision at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The whip tears flesh and snaps bone in a dance of pain!";
		} else if (weaponType == "Lamp") {
			cout << " -> You (" << attacker->getName() << ") swing the battered " << weaponName << " into " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". Sparks and splintered bone mark the savage blow!";
		} else if (weaponType == "Fishing Rod") {
			cout << " -> You (" << attacker->getName() << ") hurl " << weaponName << " at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The rod snaps with a sickening crack, leaving them reeling!";
		} else if (weaponType == "Scepter") {
			cout << " -> You (" << attacker->getName() << ") crash " << weaponName << " down upon " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The weight drives them to their knees in torment!";
		} else if (weaponType == "Spear") {
			cout << " -> You (" << attacker->getName() << ") thrust " << weaponName << " with deadly precision at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The spear pierces armor and bone, driving them to the ground in agony!";
		} else if (weaponType == "Knife") {
			cout << " -> You (" << attacker->getName() << ") slash with " << weaponName << " at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The blade cuts deep, tearing flesh and leaving a shadowy wound that seeps with cursed venom!";
		} else if (weaponType == "War Axe") {
			cout << " -> You (" << attacker->getName() << ") swing " << weaponName << " in a brutal arc at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The axe bites deep, cleaving flesh and bone with savage force!";
		} else if (weaponType == "Club") {
			cout << " -> You (" << attacker->getName() << ") smash " << weaponName << " against " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The club crushes armor and bone, leaving a brutal impact!";
		} else if (weaponType == "Claws") {
			cout << " -> You (" << attacker->getName() << ") rake " << weaponName << " across " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The claws tear through flesh and sinew, leaving savage wounds that weep crimson!";
		} else if (weaponType == "Brick") {
			cout << " -> You (" << attacker->getName() << ") hurl " << weaponName << " at " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The brick connects with a sickening crunch, leaving them dazed and bloodied!";
		} else if (weaponType == "Wrench") {
			cout << " -> You (" << attacker->getName() << ") slam " << weaponName << " into " << targetName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". The wrench connects with a bone-crunching crack, leaving them crumpled and reeling!";
		} else {
			cout << " -> You (" << attacker->getName() << ") strike " << targetName << " with " << weaponName << ", dealing " << damage << " damage";
			if (isCrit) cout << ". A vicious blow rends flesh and spirit!";
		}
		
		cout << endl;
	} else {
		Player* pT = dynamic_cast<Player*>(target);
		
		if (pT != nullptr) {
			//if player has some kind of armor, he gets less damage, as simple as that
			// armor is ignored if ArmorBreakStatusEffect is active
			// check if ArmorBreakStatusEffect (type 1) is active on the player
			bool armorBroken = false;
			for (int i = 0; i < 4; i++) {
				if (dynamic_cast<ArmorBreakStatusEffect*>(pT->getStatusEffect()[i]) != nullptr) {
					armorBroken = true;
					break;
				}
			}
			if (!armorBroken) {
				if (pT->getHelmet() != nullptr) damage = damage / 100.0 * (100.0 - pT->getHelmet()->getDamageReduction());
				if (pT->getChestplate() != nullptr) damage = damage / 100.0 * (100.0 - pT->getChestplate()->getDamageReduction());
				if (pT->getGloves() != nullptr) damage = damage / 100.0 * (100.0 - pT->getGloves()->getDamageReduction());
				if (pT->getLeggings() != nullptr) damage = damage / 100.0 * (100.0 - pT->getLeggings()->getDamageReduction());
				if (pT->getBoots() != nullptr) damage = damage / 100.0 * (100.0 - pT->getBoots()->getDamageReduction());
			}
		}
		
		damage = std::round(damage * 100.0) / 100.0;
		
		cout << " -> " << attacker->getName() << " lashes out at you (" << pT->getName() << "), rending " << damage << " damage";
		if (isCrit) cout << ". A critical strike!" ;
		
		cout << endl;
	}
}

void BattleManager::attackUsingSpell(CombatCharacter* attacker, CombatCharacter* target, int spellIndex) {
	if (attacker->getCurrentHp() <= 0) return;
	
	bool attackerIsPlayer = (dynamic_cast<Player*>(attacker) != nullptr);
	Player* pT = dynamic_cast<Player*>(target);
	
	Spell* s = attacker->getSpells()[spellIndex];
	
	//spell is invalid
	if (s == nullptr) {
		if (attackerIsPlayer) {
			cout << " -> You (" << attacker->getName() << ") reached into the void to summon a spell." << endl;
			cout << " -> But the abyss answered with silence. No such power exists." << endl;
			cout << " -> Steel it is, then. You strike with a mortal blow." << endl;
		} else {
			cout << " -> " << attacker->getName() << " whispered to unseen forces." << endl;
			cout << " -> Nothing answered. The spell was but an empty promise." << endl;
			cout << " -> " << attacker->getName() << " lunged forward with a crude, physical strike." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	//cooldown
	if (s->getRemainingCooldown() != 0) {
		if (attackerIsPlayer) {
			cout << " -> You (" << attacker->getName() << ") invoked the forbidden name of " << s->getName() << "." << endl;
			cout << " -> Yet its power still slumbers, bound by unseen chains." << endl;
			cout << " -> Frustrated, you carve your will into flesh with a basic attack." << endl;
		} else {
			cout << " -> " << attacker->getName() << " attempted to unleash " << s->getName() << "." << endl;
			cout << " -> But the spell flickered and died — its essence not yet restored." << endl;
			cout << " -> With a snarl, " << attacker->getName() << " resorts to a savage physical assault." << endl;
		}
		
		attack(attacker, target);
		return;
	}
	
	string spellType = s->getType();
	string conjureMsg;
	if(spellType == "Fire") conjureMsg = "flames ignite around " + attacker->getName() + "'s hands.";
	else if(spellType == "Blood") conjureMsg = "dark crimson energy seeps from " + attacker->getName() + "'s veins.";
	else if(spellType == "Water") conjureMsg = "water swirls and churns around " + attacker->getName() + ".";
	else if(spellType == "Dark") conjureMsg = "darkness writhes and coils around " + attacker->getName() + ".";
	else if(spellType == "Nature") conjureMsg = "thorned vines and roots spiral around " + attacker->getName() + ".";
	else if(spellType == "Light") conjureMsg = "a blinding radiance flares from " + attacker->getName() + "'s palms.";
	else if(spellType == "Metal") conjureMsg = "jagged shards of dark iron tear free from the earth around " + attacker->getName() + ".";
	else if(spellType == "Garbage") conjureMsg = "a reeking storm of refuse and rot erupts around " + attacker->getName() + ".";
	else if(spellType == "Ash") conjureMsg = "a swirling shroud of pale ash rises from the ground around " + attacker->getName() + ".";
	else if(spellType == "Smoke") conjureMsg = "thick billowing smoke coils and churns around " + attacker->getName() + ".";
	else if(spellType == "Electric") conjureMsg = "crackling arcs of lightning surge and snap around " + attacker->getName() + ".";
	else if(spellType == "Physical") conjureMsg = "raw kinetic force ripples and distorts the air around " + attacker->getName() + ".";
	else if(spellType == "Chains") conjureMsg = "iron chains rattle and coil from the shadows around " + attacker->getName() + ".";
	else if(spellType == "Rot") conjureMsg = "a festering miasma of decay and putrefaction seeps from the ground around " + attacker->getName() + ".";
	else if(spellType == "Glass") conjureMsg = "razor-thin shards of translucent glass crystallise and orbit silently around " + attacker->getName() + ".";
	else if(spellType == "Sound") conjureMsg = "the air around " + attacker->getName() + " begins to thrum and vibrate with a low, resonant frequency.";
	else if(spellType == "Explosion") conjureMsg = "volatile energy builds and crackles violently around " + attacker->getName() + ", the air itself warping from the pressure.";
	else conjureMsg = "a dark energy forms around " + attacker->getName() + "'s hands.";
	cout << " -> " << (attackerIsPlayer ? "You (" : "") << attacker->getName() << (attackerIsPlayer ? ")" : "") << " muttered the cursed words of " << s->getName()  << ", and " << conjureMsg << endl;
	
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	StatusEffectSpell* ses = dynamic_cast<StatusEffectSpell*>(s);
	LifeStealSpell* lss = dynamic_cast<LifeStealSpell*>(s);
	
	//chaining spell
	if (cs != nullptr) {
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1)
		+ cs->getMinHits();
		
		float totalDmg = hits * cs->getDmg();
		
		cout << " -> Spell hit " << hits << " times!" << endl;
		
		cout << " -> The spell lashes out " << hits << " times, shadows and fire converging!" << endl;
		
		if (pT != nullptr) {
			//if player has some kind of armor, he gets less damage, as simple as that
			// armor is ignored if ArmorBreakStatusEffect is active
			bool armorBroken = false;
			for (int i = 0; i < 4; i++) {
				if (dynamic_cast<ArmorBreakStatusEffect*>(pT->getStatusEffect()[i]) != nullptr) {
					armorBroken = true;
					break;
				}
			}
			if (!armorBroken) {
				if (pT->getHelmet() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getHelmet()->getDamageReduction());
				if (pT->getChestplate() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getChestplate()->getDamageReduction());
				if (pT->getGloves() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getGloves()->getDamageReduction());
				if (pT->getLeggings() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getLeggings()->getDamageReduction());
				if (pT->getBoots() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getBoots()->getDamageReduction());
			}
		}
		totalDmg = std::round(totalDmg * 100.0) / 100.0;
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		
		if (attackerIsPlayer) cout << " -> You (" << attacker->getName() << ") sear " << target->getName() << " for " << totalDmg << " damage." << endl;
		else	cout << " -> " << attacker->getName() << "'s spell scorches you, inflicting " << totalDmg << " damage!" << endl;
	}
	
	//status effect spell
	else if (ses != nullptr) {
		float totalDmg = s->getDmg();
		
		if (pT != nullptr) {
			//if player has some kind of armor, he gets less damage, as simple as that
			// armor is ignored if ArmorBreakStatusEffect is active
			bool armorBroken = false;
			for (int i = 0; i < 4; i++) {
				if (dynamic_cast<ArmorBreakStatusEffect*>(pT->getStatusEffect()[i]) != nullptr) {
					armorBroken = true;
					break;
				}
			}
			if (!armorBroken) {
				if (pT->getHelmet() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getHelmet()->getDamageReduction());
				if (pT->getChestplate() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getChestplate()->getDamageReduction());
				if (pT->getGloves() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getGloves()->getDamageReduction());
				if (pT->getLeggings() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getLeggings()->getDamageReduction());
				if (pT->getBoots() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getBoots()->getDamageReduction());
			}
		}
		totalDmg = std::round(totalDmg * 100.0) / 100.0;
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		
		if (attackerIsPlayer) {
			cout << " -> Your " << s->getName() << " wracks " << target->getName() << " for " << totalDmg << " damage." << endl;
		} else {
			cout << " -> " << s->getName() << " tears into you (" << target->getName() << "), dealing " << totalDmg << " damage." << endl;
		}
		
		if (rand() % 100 + 1 <= ses->getChanceToRecieve() && ses->getStatusToGive() != nullptr) {
			cout << " -> " << target->getName() << " is cursed with " << ses->getStatusToGive()->getName() << ", a lingering shadow over their vitality." << endl;
			
			target->addStatusEffect(ses->getStatusToGive());
		}
	}
	
	//lifesteal spell
	else if(lss != nullptr){
		float totalDmg = s->getDmg();
		float totalLifesteal;
		
		if (pT != nullptr) {
			//if player has some kind of armor, he gets less damage, as simple as that
			// armor is ignored if ArmorBreakStatusEffect is active
			bool armorBroken = false;
			for (int i = 0; i < 4; i++) {
				if (dynamic_cast<ArmorBreakStatusEffect*>(pT->getStatusEffect()[i]) != nullptr) {
					armorBroken = true;
					break;
				}
			}
			if (!armorBroken) {
				if (pT->getHelmet() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getHelmet()->getDamageReduction());
				if (pT->getChestplate() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getChestplate()->getDamageReduction());
				if (pT->getGloves() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getGloves()->getDamageReduction());
				if (pT->getLeggings() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getLeggings()->getDamageReduction());
				if (pT->getBoots() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getBoots()->getDamageReduction());
			}
		}
		totalDmg = std::round(totalDmg * 100.0) / 100.0;
		totalLifesteal = s->getDmg() / 100 * lss->getLifeStealRate();
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		attacker->setCurrentHp(attacker->getCurrentHp() + totalLifesteal);
		
		if (attackerIsPlayer) cout << " -> You (" << attacker->getName() << ") invoke " << s->getName() << ", draining " << target->getName() << " for " << totalDmg << " damage and siphoning " << totalLifesteal << " vitality back into yourself!" << endl;
		else cout << " -> " << attacker->getName() << " casts " << s->getName() << ", tearing " << totalDmg << " life from you and restoring " << totalLifesteal << " to itself!" << endl;
	}
	
	//normal spell
	else {
		float totalDmg = s->getDmg();
		
		if (pT != nullptr) {
			//if player has some kind of armor, he gets less damage, as simple as that
			// armor is ignored if ArmorBreakStatusEffect is active
			bool armorBroken = false;
			for (int i = 0; i < 4; i++) {
				if (dynamic_cast<ArmorBreakStatusEffect*>(pT->getStatusEffect()[i]) != nullptr) {
					armorBroken = true;
					break;
				}
			}
			if (!armorBroken) {
				if (pT->getHelmet() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getHelmet()->getDamageReduction());
				if (pT->getChestplate() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getChestplate()->getDamageReduction());
				if (pT->getGloves() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getGloves()->getDamageReduction());
				if (pT->getLeggings() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getLeggings()->getDamageReduction());
				if (pT->getBoots() != nullptr) totalDmg = totalDmg / 100.0 * (100.0 - pT->getBoots()->getDamageReduction());
			}
		}
		totalDmg = std::round(totalDmg * 100.0) / 100.0;
		
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		
		if (attackerIsPlayer) cout << " -> You (" << attacker->getName() << ") unleash the arcane fury of " << s->getName() << ", rending " << target->getName()	<< " for " << totalDmg << " damage!" << endl;
		else cout << " -> " << attacker->getName() << " strikes you with " << s->getName() << ", searing flesh and spirit for " << totalDmg << " damage!" << endl;
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}


//battle misc
void BattleManager::roundIncrement() {
	this->round++;
	
	for (int i = 0; i < 4; i++) {
		if (this->player->getSpells()[i] != nullptr) {
			if (this->player->getSpells()[i]->getRemainingCooldown() != 0) {
				this->player->getSpells()[i]->setRemainingCooldown(this->player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
		
		if (this->enemy->getSpells()[i] != nullptr) {
			if (this->enemy->getSpells()[i]->getRemainingCooldown() != 0) {
				this->enemy->getSpells()[i]->setRemainingCooldown(this->enemy->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
	}
}

void BattleManager::checkDeaths() {
	if (this->player->getCurrentHp() <= 0) {
		this->player->setCurrentHp(0);
		this->isFinished = 1;
		
		cout << "==========================================" << endl;
		cout << " -> You (" << this->player->getName() << ") have fallen..." << endl;
		cout << " -> The darkness swallows you whole." << endl;
		cout << "==========================================" << endl;
		
		system("pause");
		
	} else if (this->enemy->getCurrentHp() <= 0) {
		this->enemy->setCurrentHp(0);
		this->isFinished = 2;
		
		Player* p = dynamic_cast<Player*>(this->player);
		Enemy*  e = dynamic_cast<Enemy*>(this->enemy);
		
		cout << "==========================================" << endl;
		cout << " -> Victory! You (" << p->getName() << ") stand over the fallen " << e->getName() << "." << endl;
		cout << " -> Their life force fuels your ascent." << endl;
		cout << "==========================================" << endl;
		
		// XP
		float xpGained = e->getXpToGet();
		p->setCurrentXp(p->getCurrentXp() + xpGained);
		cout << " -> You gained " << xpGained << " XP." << endl;
		
		// Soul Stone drop
		int ssChance = e->getSoulStoneDropChance();
		if (ssChance > 0) {
			int ssRoll = rand() % 100 + 1;
			if (ssRoll <= ssChance) {
				if (p->getSoulStones() < 9) {
					p->addSoulStone();
					cout << endl;
					cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
					cout << " -> A faint glimmer escapes the corpse." << endl;
					cout << " -> You reach out. A Soul Stone — cold and humming — settles in your palm." << endl;
					cout << " -> Soul Stones: ";
					for (int i = 0; i < p->getSoulStones(); i++) cout << "[*] ";
					cout << endl;
					cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				} else {
					cout << " -> A Soul Stone glows at your feet, but your spirit is already full." << endl;
					cout << " -> It crumbles into ash." << endl;
				}
			}
		}
		
		// Loot
		vector<Item*>& items   = e->getRewardItems();
		vector<int>&   chances = e->getRewardChances();
		
		cout << " -> Loot:" << endl;
		bool anyDrop = false;
		for (size_t i = 0; i < items.size(); i++) {
			int roll = rand() % 100 + 1;
			if (roll <= chances[i]) {
				cout << "     - " << items[i]->getName() << endl;
				p->getInventory().addItem(items[i]);
				anyDrop = true;
			}
		}
		if (!anyDrop) cout << "     - Nothing. The creature carried only spite." << endl;
		
		system("pause");
		
	} else {
		this->isFinished = 0;
	}
}

void BattleManager::checkStatusEffects() {
	HpStatusEffect* hps;
	
	bool playerFirst = this->player->getSpeed() >= this->enemy->getSpeed();
	
	for (int i = 0; i < 4; i++) {
		CombatCharacter* first  = playerFirst ? (CombatCharacter*)player : (CombatCharacter*)enemy;
		CombatCharacter* second = playerFirst ? (CombatCharacter*)enemy : (CombatCharacter*)player;
		
		for (int turn = 0; turn < 2; turn++) {
			CombatCharacter* current = (turn == 0 ? first : second);
			
			StatusEffect* se = current->getStatusEffect()[i];
			if (se == nullptr) continue;
			
			bool isPlayer = (current == player);
			
			// ArmorBreakStatusEffect (type 1): no damage, just reminds player their armor is disabled
			ArmorBreakStatusEffect* abs = dynamic_cast<ArmorBreakStatusEffect*>(se);
			if (abs != nullptr) {
				if (isPlayer) {
					cout << "==========================================" << endl;
					int remaining = abs->getCurrentDuration() - abs->getCurrentRound() - 1;
					if (remaining > 0)
						cout << " -> " << abs->getName() << " holds — your armor is powerless against the blows. (" << remaining << " round(s) left)" << endl;
					else
						cout << " -> " << abs->getName() << " fades away. Your armor stands firm once more." << endl;
				}
				abs->increaseRound();
				continue;
			}
			
			// TauntStatusEffect (type 2): no damage, reminds the holder they cannot cast spells
			TauntStatusEffect* ts = dynamic_cast<TauntStatusEffect*>(se);
			if (ts != nullptr) {
				cout << "==========================================" << endl;
				int remaining = ts->getCurrentDuration() - ts->getCurrentRound() - 1;
				if (isPlayer) {
					if (remaining > 0)
						cout << " -> " << ts->getName() << " burns in your blood — your spells slip from your mind. (" << remaining << " round(s) left)" << endl;
					else
						cout << " -> The " << ts->getName() << " subsides. Your focus returns — spells are yours once more." << endl;
				} else {
					if (remaining > 0)
						cout << " -> " << ts->getName() << " holds " << enemy->getName() << " in its grip — its spells are out of reach. (" << remaining << " round(s) left)" << endl;
					else
						cout << " -> The " << ts->getName() << " on " << enemy->getName() << " wears off. It regains its arcane focus." << endl;
				}
				ts->increaseRound();
				continue;
			}
			
			hps = dynamic_cast<HpStatusEffect*>(se);
			if (hps == nullptr) continue;
			
			current->setCurrentHp(current->getCurrentHp() + hps->getHpAffection());
			
			cout << "==========================================" << endl;
			
			if (hps->getHpAffection() < 0.0) {
				if (isPlayer)
					cout << " -> Dark forces from " << hps->getName() << " bite you, dealing " << -hps->getHpAffection() << " damage." << endl;
				else
					cout << " -> " << enemy->getName() << " suffers from " << hps->getName() << ", losing " << -hps->getHpAffection() << " hp." << endl;
			} else {
				if (isPlayer)
					cout << " -> A gentle light of " << hps->getName() << " heals you for " << hps->getHpAffection() << " hp." << endl;
				else
					cout << " -> " << enemy->getName() << " is restored by " << hps->getName() << " for " << hps->getHpAffection() << " hp." << endl;
			}
		}
	}
	
	// remove any status effects that have run their full duration
	this->player->checkStatusEffects();
	this->enemy->checkStatusEffects();
}

//the battle itself
void BattleManager::battle() {
	int choice;
	srand(time(0));
	
	while (this->isFinished == 0) {
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
		cout << " 3. Use consumable" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Choose your fate, adventurer: ";
		cin >> choice;
		cout << "==========================================" << endl << endl;
		system("cls");
		
		switch (choice) {
			case 1: {
				cout << "==========================================" << endl;
				if (player->getSpeed() >= enemy->getSpeed()) {
					attack(player, enemy);
					cout << endl;
					
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					bool enemyTaunted = false;
					for (int ti = 0; ti < 4; ti++) {
						if (dynamic_cast<TauntStatusEffect*>(enemy->getStatusEffect()[ti]) != nullptr) {
							enemyTaunted = true; break;
						}
					}
					if (enemyTaunted || randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0) {
						attack(enemy, player);
					} else {
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
					}
				} else {
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					bool enemyTaunted = false;
					for (int ti = 0; ti < 4; ti++) {
						if (dynamic_cast<TauntStatusEffect*>(enemy->getStatusEffect()[ti]) != nullptr) {
							enemyTaunted = true; break;
						}
					}
					if (enemyTaunted || randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0) {
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
				// check if player is taunted - if so, force a normal attack instead
				bool playerTaunted = false;
				for (int i = 0; i < 4; i++) {
					if (dynamic_cast<TauntStatusEffect*>(player->getStatusEffect()[i]) != nullptr) {
						playerTaunted = true;
						break;
					}
				}
				
				if (playerTaunted) {
					cout << "==========================================" << endl;
					cout << " -> The " << player->getName() << " is gripped by the Taunt — the words of your spells dissolve before they form." << endl;
					cout << " -> Your body acts on instinct, hurling itself forward into a basic strike." << endl;
					cout << "==========================================" << endl;
					
					if (player->getSpeed() >= enemy->getSpeed()) {
						attack(player, enemy);
						cout << endl;
						Spell* es = enemy->getSpells()[randomiseSpellEnemy];
						if (randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
							attack(enemy, player);
						else
							attackUsingSpell(enemy, player, randomiseSpellEnemy);
					} else {
						Spell* es = enemy->getSpells()[randomiseSpellEnemy];
						if (randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
							attack(enemy, player);
						else
							attackUsingSpell(enemy, player, randomiseSpellEnemy);
						cout << endl;
						attack(player, enemy);
					}
					break;
				}
				
				Spell* s;
				ChainingSpell* cs;
				StatusEffectSpell* sas;
				LifeStealSpell* lss;
				
				cout << "==========================================" << endl;
				cout << *player;
				cout << "------------------------------------------" << endl;
				cout << *enemy;
				cout << "==========================================" << endl;
				
				cout << "Your spells, etched into your soul:" << endl << endl;
				for (int i = 0; i < 4; i++) {
					Spell *s = player->getSpells()[i];
					
					cout << " " << i + 1 << ". ";
					cout << "Etched into your soul: ";
					if (s == nullptr) {
						cout << "A hollow space within your mind." << endl << endl;
					} else {
						ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
						StatusEffectSpell* sas = dynamic_cast<StatusEffectSpell*>(s);
						LifeStealSpell* lss = dynamic_cast<LifeStealSpell*>(s);
						
						if (cs != nullptr) {
							cout << *cs;
						} else if (sas != nullptr) {
							cout << *sas;
						} else if (lss != nullptr) {
							cout << *lss;
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
				
				while (choice > 4) choice -= 4;
				
				cout << "==========================================" << endl;
				if (player->getSpeed() >= enemy->getSpeed()) {
					attackUsingSpell(player, enemy, choice - 1);
					cout << endl;
					
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					bool enemyTaunted = false;
					for (int ti = 0; ti < 4; ti++) {
						if (dynamic_cast<TauntStatusEffect*>(enemy->getStatusEffect()[ti]) != nullptr) {
							enemyTaunted = true; break;
						}
					}
					if (enemyTaunted || randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
						attack(enemy, player);
					else
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
				} else {
					Spell* es = enemy->getSpells()[randomiseSpellEnemy];
					bool enemyTaunted = false;
					for (int ti = 0; ti < 4; ti++) {
						if (dynamic_cast<TauntStatusEffect*>(enemy->getStatusEffect()[ti]) != nullptr) {
							enemyTaunted = true; break;
						}
					}
					if (enemyTaunted || randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0)
						attack(enemy, player);
					else
						attackUsingSpell(enemy, player, randomiseSpellEnemy);
					
					cout << endl;
					attackUsingSpell(player, enemy, choice - 1);
				}
				
				break;
			}
			
			case 3: {
				Player* p = dynamic_cast<Player*>(player);
				vector<Item*>& items = p->getInventory().getStorage();
				vector<int> consumableIndexes;
				
				
				cout << "==========================================" << endl;
				cout << *player;
				cout << "------------------------------------------" << endl;
				cout << *enemy;
				cout << "==========================================" << endl;
				cout << "Provisions in your pack, worn from the road:" << endl << endl;
				
				for (size_t i = 0; i < items.size(); i++) {
					Consumable* c = dynamic_cast<Consumable*>(items[i]);
					if (c != nullptr) {
						cout << consumableIndexes.size() + 1 << ". " << *c << endl;
						consumableIndexes.push_back(i);
					}
				}
				
				if (consumableIndexes.empty()) {
					cout << " -> You search your pack, but find nothing left to use." << endl;
					system("pause");
					system("cls");
					break;
				}
				
				cout << "==========================================" << endl;
				cout << "Which provision will you use? ";
				cin >> choice;
				cout << "==========================================" << endl;
				
				system("cls");
				
				if (choice <= 0 || choice > consumableIndexes.size()) {
					cout << " -> You put it away. The moment to act passes." << endl;
					break;
				}
				
				int realIndex = consumableIndexes[choice - 1];
				Consumable* used = dynamic_cast<Consumable*>(items[realIndex]);
				
				if (used != nullptr) {
					float healAmount = used->getHpToRecover();
					
					player->setCurrentHp(player->getCurrentHp() + healAmount);
					if (player->getCurrentHp() > player->getMaxHp()) player->setCurrentHp(player->getMaxHp());
					
					cout << "==========================================" << endl;
					cout << " -> You use " << used->getName() << ". " << healAmount << " vitality flows back into your veins." << endl;
					
					cout << realIndex;
					items.erase(items.begin() +  realIndex);
				} else {
					cout << " -> That item cannot be used." << endl;
				}
				
				cout << endl;
				
				
				Spell* es = enemy->getSpells()[randomiseSpellEnemy];
				bool enemyTaunted = false;
				for (int ti = 0; ti < 4; ti++) {
					if (dynamic_cast<TauntStatusEffect*>(enemy->getStatusEffect()[ti]) != nullptr) {
						enemyTaunted = true; break;
					}
				}
				if (enemyTaunted || randomiseAttackEnemy < 7 || es == nullptr || es->getRemainingCooldown() > 0) attack(enemy, player);
				else attackUsingSpell(enemy, player, randomiseSpellEnemy);
				break;
			}	
			
		default:
			break;
		}
		
		roundIncrement();
		checkStatusEffects();
		checkDeaths();
	}
}
