#include "groupbattlemanager.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;

// ============================================================
//  CONSTRUCTORS
// ============================================================
GroupBattleManager::GroupBattleManager() {
	this->round      = 0;
	this->isFinished = 0;
	this->player     = nullptr;
}
GroupBattleManager::GroupBattleManager(CombatCharacter* player, vector<Enemy*> enemies) {
	this->round      = 0;
	this->isFinished = 0;
	this->player     = player;
	this->enemies    = enemies;
}

// ============================================================
//  GETTERS / SETTERS
// ============================================================
int  GroupBattleManager::getRound()       { return this->round; }
int  GroupBattleManager::getIsFinished()  { return this->isFinished; }
CombatCharacter* GroupBattleManager::getPlayer()   { return this->player; }
vector<Enemy*>&  GroupBattleManager::getEnemies()  { return this->enemies; }

void GroupBattleManager::setRound(int round)           { this->round = round; }
void GroupBattleManager::setIsFinished(int isFinished) { this->isFinished = isFinished; }
void GroupBattleManager::setPlayer(CombatCharacter* player) { this->player = player; }

// ============================================================
//  HELPERS
// ============================================================
vector<Enemy*> GroupBattleManager::livingEnemies() {
	vector<Enemy*> alive;
	for (Enemy* e : enemies)
		if (e->getCurrentHp() > 0) alive.push_back(e);
	return alive;
}

bool GroupBattleManager::isArmorBroken(Player* target) {
	for (int i = 0; i < 4; i++)
		if (dynamic_cast<ArmorBreakStatusEffect*>(target->getStatusEffect()[i]) != nullptr)
			return true;
	return false;
}

void GroupBattleManager::applyArmorReduction(float& dmg, Player* target) {
	if (isArmorBroken(target)) return;
	if (target->getHelmet()     != nullptr) dmg = dmg / 100.0f * (100.0f - target->getHelmet()->getDamageReduction());
	if (target->getChestplate() != nullptr) dmg = dmg / 100.0f * (100.0f - target->getChestplate()->getDamageReduction());
	if (target->getGloves()     != nullptr) dmg = dmg / 100.0f * (100.0f - target->getGloves()->getDamageReduction());
	if (target->getLeggings()   != nullptr) dmg = dmg / 100.0f * (100.0f - target->getLeggings()->getDamageReduction());
	if (target->getBoots()      != nullptr) dmg = dmg / 100.0f * (100.0f - target->getBoots()->getDamageReduction());
}

string GroupBattleManager::conjureMessage(CombatCharacter* attacker, const string& spellType) {
	if(spellType == "Fire") return "flames ignite around " + attacker->getName() + "'s hands.";
	else if(spellType == "Blood") return "dark crimson energy seeps from " + attacker->getName() + "'s veins.";
	else if(spellType == "Water") return "water swirls and churns around " + attacker->getName() + ".";
	else if(spellType == "Dark") return "darkness writhes and coils around " + attacker->getName() + ".";
	else if(spellType == "Nature") return "thorned vines and roots spiral around " + attacker->getName() + ".";
	else if(spellType == "Light") return "a blinding radiance flares from " + attacker->getName() + "'s palms.";
	else if(spellType == "Metal") return "jagged shards of dark iron tear free from the earth around " + attacker->getName() + ".";
	else if(spellType == "Garbage") return "a reeking storm of refuse and rot erupts around " + attacker->getName() + ".";
	else if(spellType == "Ash") return "a swirling shroud of pale ash rises from the ground around " + attacker->getName() + ".";
	else if(spellType == "Smoke") return "thick billowing smoke coils and churns around " + attacker->getName() + ".";
	else if(spellType == "Electric") return "crackling arcs of lightning surge and snap around " + attacker->getName() + ".";
	else if(spellType == "Physical") return "raw kinetic force ripples and distorts the air around " + attacker->getName() + ".";
	else if(spellType == "Chains") return "iron chains rattle and coil from the shadows around " + attacker->getName() + ".";
	else if(spellType == "Rot") return "a festering miasma of decay and putrefaction seeps from the ground around " + attacker->getName() + ".";
	else if(spellType == "Glass") return "razor-thin shards of translucent glass crystallise and orbit silently around " + attacker->getName() + ".";
	else if(spellType == "Sound") return "the air around " + attacker->getName() + " begins to thrum and vibrate with a low, resonant frequency.";
	else if(spellType == "Explosion") return "volatile energy builds and crackles violently around " + attacker->getName() + ", the air itself warping from the pressure.";
	else if(spellType == "Molten metal") return "viscous streams of glowing molten metal seep and coil around " + attacker->getName() + ", dripping heat that scorches the ground beneath.";
	else if(spellType == "Magnetic") return "loose metal trembles and lifts as unseen forces twist around " + attacker->getName() + ", orbiting in a violent, unseen pull.";
	else if(spellType == "Ground") return "the earth beneath " + attacker->getName() + " cracks and heaves, stones grinding as something deep below answers their call.";
	else if(spellType == "Ice") return "frozen shards shiver and hover as frigid winds coil around " + attacker->getName() + ", piercing the air with lethal precision.";
	else if(spellType == "Rune") return "etched sigils flare to life as ancient energy coils around " + attacker->getName() + ", their glow cutting through the air with precise, lethal intent.";
	else return "a dark energy forms around " + attacker->getName() + "'s hands.";
}

// ============================================================
//  PRINT BATTLE STATE
// ============================================================
void GroupBattleManager::printBattleState() {
	Player* p = dynamic_cast<Player*>(player);
	cout << "==========================================" << endl;
	cout << *p;
	cout << "------------------------------------------" << endl;
	for (int i = 0; i < (int)enemies.size(); i++) {
		Enemy* e = enemies[i];
		if (e->getCurrentHp() <= 0) {
			cout << "  " << i + 1 << ". " << e->getName() << " — defeated" << endl;
		} else {
			cout << "  " << i + 1 << ". " << e->getName()
			<< " | " << e->getCurrentHp() << "/" << e->getMaxHp() << " hp"
			<< " | dmg " << e->getDmg()
			<< " | spd " << e->getSpeed() << endl;
		}
	}
	cout << "==========================================" << endl;
}

// ============================================================
//  PICK TARGET
// ============================================================
int GroupBattleManager::pickTarget(const string& prompt) {
	if (tauntTarget != nullptr && tauntTarget->getCurrentHp() > 0) {
		cout << " -> You are compelled by the taunt — you must target " << tauntTarget->getName() << "!" << endl;
		for (int i = 0; i < (int)enemies.size(); i++)
			if (enemies[i] == tauntTarget) return i;
	}
	
	vector<int> aliveIdx;
	cout << prompt << endl;
	for (int i = 0; i < (int)enemies.size(); i++) {
		if (enemies[i]->getCurrentHp() > 0) {
			aliveIdx.push_back(i);
			cout << "  " << aliveIdx.size() << ". " << enemies[i]->getName()
			<< " (" << enemies[i]->getCurrentHp() << "/" << enemies[i]->getMaxHp() << " hp)" << endl;
		}
	}
	cout << "Target: ";
	int choice; cin >> choice;
	if (choice < 1 || choice > (int)aliveIdx.size()) choice = 1;
	return aliveIdx[choice - 1];
}

// ============================================================
//  PLAYER ATTACK — basic
// ============================================================
void GroupBattleManager::playerAttack(Enemy* target) {
	if (player->getCurrentHp() <= 0) return;
	
	Player* p = dynamic_cast<Player*>(player);
	float dmg = player->getDmg();
	if (p != nullptr) dmg += p->getWeapon()->getDmg();
	
	int crit = rand() % 100 + 1;
	bool isCrit = false;
	if (crit <= player->getCritChance()) {
		dmg *= player->getCritValue();
		isCrit = true;
	}
	
	target->setCurrentHp(target->getCurrentHp() - dmg);
	
	if (p != nullptr) {
		string weaponName = p->getWeapon()->getName();
		string weaponType = p->getWeapon()->getType();
		
		if (weaponType == "Sword") {
			cout << " -> You (" << player->getName() << ") swing " << weaponName << " in a deadly arc at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The strike cleaves bone and shadow alike!";
		} else if (weaponType == "Mace") {
			cout << " -> You (" << player->getName() << ") slam " << weaponName << " down upon " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The impact rattles armor and cracks sinew!";
		} else if (weaponType == "Whip") {
			cout << " -> You (" << player->getName() << ") lash " << weaponName << " with serpentine precision at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The whip tears flesh and snaps bone in a dance of pain!";
		} else if (weaponType == "Lamp") {
			cout << " -> You (" << player->getName() << ") swing the battered " << weaponName << " into " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". Sparks and splintered bone mark the savage blow!";
		} else if (weaponType == "Fishing Rod") {
			cout << " -> You (" << player->getName() << ") hurl " << weaponName << " at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The rod snaps with a sickening crack, leaving them reeling!";
		} else if (weaponType == "Scepter") {
			cout << " -> You (" << player->getName() << ") crash " << weaponName << " down upon " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The weight drives them to their knees in torment!";
		} else if (weaponType == "Spear") {
			cout << " -> You (" << player->getName() << ") thrust " << weaponName << " with deadly precision at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The spear pierces armor and bone, driving them to the ground in agony!";
		} else if (weaponType == "Knife") {
			cout << " -> You (" << player->getName() << ") slash with " << weaponName << " at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The blade cuts deep, tearing flesh and leaving a shadowy wound that seeps with cursed venom!";
		} else if (weaponType == "War Axe") {
			cout << " -> You (" << player->getName() << ") swing " << weaponName << " in a brutal arc at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The axe bites deep, cleaving flesh and bone with savage force!";
		} else if (weaponType == "Club") {
			cout << " -> You (" << player->getName() << ") smash " << weaponName << " against " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The club crushes armor and bone, leaving a brutal impact!";
		} else if (weaponType == "Claws") {
			cout << " -> You (" << player->getName() << ") rake " << weaponName << " across " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The claws tear through flesh and sinew, leaving savage wounds that weep crimson!";
		} else if (weaponType == "Brick") {
			cout << " -> You (" << player->getName() << ") hurl " << weaponName << " at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The brick connects with a sickening crunch, leaving them dazed and bloodied!";
		} else if (weaponType == "Wrench") {
			cout << " -> You (" << player->getName() << ") slam " << weaponName << " into " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The wrench connects with a bone-crunching crack, leaving them crumpled and reeling!";
		} else if (weaponType == "Guitar Axe") {
			cout << " -> You (" << player->getName() << ") bring " << weaponName << " down in a heavy, screaming swing at " << target->getName() << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". The impact howls like a broken chord, splitting them apart!";
		} else if (weaponType == "Pliers") {
			cout << " -> You (" << player->getName() << ") clamp " << weaponName << " onto " << target->getName() << ", wrenching hard for " << dmg << " damage";
			if (isCrit) cout << ". Something gives way with a sickening snap!";
		} else {
			cout << " -> You (" << player->getName() << ") strike " << target->getName() << " with " << weaponName << ", dealing " << dmg << " damage";
			if (isCrit) cout << ". A vicious blow rends flesh and spirit!";
		}
		
		cout << endl;
	}
}

// ============================================================
//  PLAYER ATTACK — spell
// ============================================================
void GroupBattleManager::playerAttackUsingSpell(Enemy* target, int spellIndex) {
	if (player->getCurrentHp() <= 0) return;
	
	Spell* s = player->getSpells()[spellIndex];
	
	if (s == nullptr) {
		cout << " -> You (" << player->getName() << ") reached into the void to summon a spell." << endl;
		cout << " -> But the abyss answered with silence. No such power exists." << endl;
		cout << " -> Steel it is, then. You strike with a mortal blow." << endl;
		playerAttack(target);
		return;
	}
	
	if (s->getRemainingCooldown() != 0) {
		cout << " -> You (" << player->getName() << ") invoked the forbidden name of " << s->getName() << "." << endl;
		cout << " -> Yet its power still slumbers, bound by unseen chains." << endl;
		cout << " -> Frustrated, you carve your will into flesh with a basic attack." << endl;
		playerAttack(target);
		return;
	}
	
	string spellType = s->getType();
	string conjureMsg = conjureMessage(player, spellType);
	cout << " -> You (" << player->getName() << ") muttered the cursed words of " << s->getName() << ", and " << conjureMsg << endl;
	
	ChainingSpell* cs = dynamic_cast<ChainingSpell*>(s);
	StatusEffectSpell* ses = dynamic_cast<StatusEffectSpell*>(s);
	LifeStealSpell* lss = dynamic_cast<LifeStealSpell*>(s);
	
	if (cs != nullptr) {
		int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
		float totalDmg = hits * cs->getDmg();
		totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		cout << " -> Spell hit " << hits << " times!" << endl;
		cout << " -> The spell lashes out " << hits << " times, shadows and fire converging!" << endl;
		cout << " -> You (" << player->getName() << ") sear " << target->getName() << " for " << totalDmg << " damage." << endl;
	} else if (ses != nullptr) {
		float totalDmg = s->getDmg();
		totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		cout << " -> Your " << s->getName() << " wracks " << target->getName() << " for " << totalDmg << " damage." << endl;
		if (rand() % 100 + 1 <= ses->getChanceToRecieve() && ses->getStatusToGive() != nullptr) {
			cout << " -> " << target->getName() << " is cursed with " << ses->getStatusToGive()->getName() << ", a lingering shadow over their vitality." << endl;
			target->addStatusEffect(ses->getStatusToGive());
		}
	} else if (lss != nullptr) {
		float totalDmg = s->getDmg();
		float totalLifesteal = s->getDmg() / 100.0f * lss->getLifeStealRate();
		totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
		totalLifesteal = std::round(totalLifesteal * 100.0f) / 100.0f;
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		player->setCurrentHp(player->getCurrentHp() + totalLifesteal);
		cout << " -> You (" << player->getName() << ") invoke " << s->getName() << ", draining " << target->getName() << " for " << totalDmg << " damage and siphoning " << totalLifesteal << " vitality back into yourself!" << endl;
	} else {
		float totalDmg = s->getDmg();
		totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
		target->setCurrentHp(target->getCurrentHp() - totalDmg);
		cout << " -> You (" << player->getName() << ") unleash the arcane fury of " << s->getName() << ", rending " << target->getName() << " for " << totalDmg << " damage!" << endl;
	}
	
	s->setRemainingCooldown(s->getFullCooldown());
}

// ============================================================
//  PLAYER USE ITEM
// ============================================================
void GroupBattleManager::playerUseItem(int itemIndex, Enemy* target) {
	Player* p = dynamic_cast<Player*>(player);
	vector<Item*>& items = p->getInventory().getStorage();
	
	if (itemIndex < 0 || itemIndex >= (int)items.size()) return;
	
	Consumable*  used   = dynamic_cast<Consumable*>(items[itemIndex]);
	AttackGadget* usedAG = dynamic_cast<AttackGadget*>(items[itemIndex]);
	
	if (used != nullptr) {
		float healAmount = used->getHpToRecover();
		player->setCurrentHp(player->getCurrentHp() + healAmount);
		if (player->getCurrentHp() > player->getMaxHp()) player->setCurrentHp(player->getMaxHp());
		cout << " -> You use " << used->getName() << ". " << healAmount << " vitality flows back into your veins." << endl;
		items.erase(items.begin() + itemIndex);
	} else if (usedAG != nullptr) {
		int backfireRoll = rand() % 100 + 1;
		string agType = usedAG->getType();
		
		if (backfireRoll <= usedAG->getChanceOfBackfire()) {
			float selfDmg = usedAG->getDamage() * 0.5f;
			player->setCurrentHp(player->getCurrentHp() - selfDmg);
			cout << " -> You hurl " << usedAG->getName() << " — but it detonates in your hand." << endl;
			cout << " -> The blast tears back into you for " << selfDmg << " damage." << endl;
		} else {
			target->setCurrentHp(target->getCurrentHp() - usedAG->getDamage());
			cout << " -> You hurl " << usedAG->getName() << " at " << target->getName() << "." << endl;
			cout << " -> It connects. " << usedAG->getDamage() << " damage tears through them." << endl;
		}
		items.erase(items.begin() + itemIndex);
	}
}

// ============================================================
//  SINGLE ENEMY TAKES THEIR TURN
// ============================================================
void GroupBattleManager::enemyTakeTurn(Enemy* e) {
	if (e->getCurrentHp() <= 0) return;
	
	Player* pT = dynamic_cast<Player*>(player);
	
	int randomAttack = rand() % 10;
	int randomSpell  = rand() % 4;
	
	bool eTaunted = false;
	for (int i = 0; i < 4; i++)
		if (dynamic_cast<TauntStatusEffect*>(e->getStatusEffect()[i]) != nullptr) { eTaunted = true; break; }
	
	Spell* es = e->getSpells()[randomSpell];
	bool useSpell = !eTaunted && randomAttack >= 7 && es != nullptr && es->getRemainingCooldown() == 0;
	
	if (!useSpell) {
		float dmg = e->getDmg();
		int crit = rand() % 100 + 1;
		bool isCrit = false;
		
		if (crit <= e->getCritChance()) {
			dmg *= e->getCritValue();
			isCrit = true;
		}
		
		if (pT != nullptr) applyArmorReduction(dmg, pT);
		dmg = std::round(dmg * 100.0f) / 100.0f;
		
		player->setCurrentHp(player->getCurrentHp() - dmg);
		cout << " -> " << e->getName() << " lashes out at you (" << pT->getName() << "), rending " << dmg << " damage";
		if (isCrit) cout << ". A critical strike!" ;
		cout << endl;
	} else {
		cout << " -> " << e->getName() << " muttered the cursed words of " << es->getName()
		<< ", and " << conjureMessage(e, es->getType()) << endl;
		
		ChainingSpell* cs = dynamic_cast<ChainingSpell*>(es);
		StatusEffectSpell* ses = dynamic_cast<StatusEffectSpell*>(es);
		LifeStealSpell* lss = dynamic_cast<LifeStealSpell*>(es);
		
		if (cs != nullptr) {
			int hits = rand() % (cs->getMaxHits() - cs->getMinHits() + 1) + cs->getMinHits();
			float totalDmg = hits * cs->getDmg();
			
			cout << " -> Spell hit " << hits << " times!" << endl;
			cout << " -> The spell lashes out " << hits << " times, shadows and fire converging!" << endl;
			
			if (pT != nullptr) applyArmorReduction(totalDmg, pT);
			totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
			
			player->setCurrentHp(player->getCurrentHp() - totalDmg);
			cout << " -> " << e->getName() << "'s spell scorches you, inflicting " << totalDmg << " damage!" << endl;
		} else if (ses != nullptr) {
			float totalDmg = es->getDmg();
			
			if (pT != nullptr) applyArmorReduction(totalDmg, pT);
			totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
			
			player->setCurrentHp(player->getCurrentHp() - totalDmg);
			cout << " -> " << es->getName() << " tears into you (" << pT->getName() << "), dealing " << totalDmg << " damage." << endl;
			
			if (rand() % 100 + 1 <= ses->getChanceToRecieve() && ses->getStatusToGive() != nullptr) {
				TauntStatusEffect* tauntSe = dynamic_cast<TauntStatusEffect*>(ses->getStatusToGive());
				if (tauntSe != nullptr) {
					tauntTarget    = e;
					tauntRoundsLeft = tauntSe->getMaxDuration();
					cout << " -> " << player->getName() << " is compelled by " << ses->getStatusToGive()->getName()
					<< " — forced to target " << e->getName() << " for " << tauntRoundsLeft << " round(s)!" << endl;
				}
				cout << " -> " << player->getName() << " is cursed with " << ses->getStatusToGive()->getName() << ", a lingering shadow over their vitality." << endl;
				player->addStatusEffect(ses->getStatusToGive());
			}
		} else if (lss != nullptr) {
			float totalDmg  = es->getDmg();
			float totalHeal = es->getDmg() / 100.0f * lss->getLifeStealRate();
			
			if (pT != nullptr) applyArmorReduction(totalDmg, pT);
			totalDmg  = std::round(totalDmg  * 100.0f) / 100.0f;
			totalHeal = std::round(totalHeal * 100.0f) / 100.0f;
			
			player->setCurrentHp(player->getCurrentHp() - totalDmg);
			e->setCurrentHp(e->getCurrentHp() + totalHeal);
			cout << " -> " << e->getName() << " casts " << es->getName() << ", tearing " << totalDmg
			<< " life from you and restoring " << totalHeal << " to itself!" << endl;
		} else {
			float totalDmg = es->getDmg();
			
			if (pT != nullptr) applyArmorReduction(totalDmg, pT);
			totalDmg = std::round(totalDmg * 100.0f) / 100.0f;
			
			player->setCurrentHp(player->getCurrentHp() - totalDmg);
			cout << " -> " << e->getName() << " strikes you with " << es->getName()
			<< ", searing flesh and spirit for " << totalDmg << " damage!" << endl;
		}
		
		es->setRemainingCooldown(es->getFullCooldown());
	}
}

// ============================================================
//  ALL ENEMIES ACT
// ============================================================
void GroupBattleManager::allEnemiesAct() {
	vector<int> order;
	for (int i = 0; i < (int)enemies.size(); i++)
		if (enemies[i]->getCurrentHp() > 0) order.push_back(i);
	
	for (int i = 0; i < (int)order.size() - 1; i++)
		for (int j = i + 1; j < (int)order.size(); j++)
			if (enemies[order[j]]->getSpeed() > enemies[order[i]]->getSpeed())
				swap(order[i], order[j]);
	
	for (int idx : order) {
		enemyTakeTurn(enemies[idx]);
	}
}

// ============================================================
//  ROUND INCREMENT
// ============================================================
void GroupBattleManager::roundIncrement() {
	this->round++;
	
	for (int i = 0; i < 4; i++) {
		if (this->player->getSpells()[i] != nullptr) {
			if (this->player->getSpells()[i]->getRemainingCooldown() != 0) {
				this->player->getSpells()[i]->setRemainingCooldown(this->player->getSpells()[i]->getRemainingCooldown() - 1);
			}
		}
		
		for (Enemy* e : enemies) {
			if (e->getSpells()[i] != nullptr) {
				if (e->getSpells()[i]->getRemainingCooldown() != 0) {
					e->getSpells()[i]->setRemainingCooldown(e->getSpells()[i]->getRemainingCooldown() - 1);
				}
			}
		}
	}
	
	if (tauntTarget != nullptr) {
		tauntRoundsLeft--;
		if (tauntRoundsLeft <= 0 || tauntTarget->getCurrentHp() <= 0) {
			if (tauntTarget->getCurrentHp() > 0)
				cout << " -> The taunt on " << tauntTarget->getName() << " fades. You may choose your target freely." << endl;
			tauntTarget    = nullptr;
			tauntRoundsLeft = 0;
		}
	}
}

// ============================================================
//  CHECK STATUS EFFECTS
// ============================================================
void GroupBattleManager::checkStatusEffects() {
	HpStatusEffect* hps;
	
	bool playerFirst = this->player->getSpeed() >= this->enemies[0]->getSpeed();
	for (size_t idx = 1; idx < this->enemies.size(); idx++)
		if (this->enemies[idx]->getSpeed() > this->player->getSpeed())
			playerFirst = false;
	
	for (int i = 0; i < 4; i++) {
		CombatCharacter* first  = playerFirst ? (CombatCharacter*)player : (CombatCharacter*)enemies[0];
		CombatCharacter* second = playerFirst ? (CombatCharacter*)enemies[0] : (CombatCharacter*)player;
		
		// Process all enemies' status effects
		for (Enemy* e : enemies) {
			StatusEffect* se = e->getStatusEffect()[i];
			if (se == nullptr) continue;
			
			bool isPlayer = false;
			
			ArmorBreakStatusEffect* abs = dynamic_cast<ArmorBreakStatusEffect*>(se);
			if (abs != nullptr) {
				if (!isPlayer) {
					cout << "==========================================" << endl;
					int remaining = abs->getCurrentDuration() - abs->getCurrentRound() - 1;
					if (remaining > 0)
						cout << " -> " << abs->getName() << " holds — " << e->getName() << "'s armor is powerless against the blows. (" << remaining << " round(s) left)" << endl;
					else
						cout << " -> " << abs->getName() << " fades away. " << e->getName() << "'s armor stands firm once more." << endl;
				}
				abs->increaseRound();
				continue;
			}
			
			TauntStatusEffect* ts = dynamic_cast<TauntStatusEffect*>(se);
			if (ts != nullptr) {
				cout << "==========================================" << endl;
				int remaining = ts->getCurrentDuration() - ts->getCurrentRound() - 1;
				if (!isPlayer) {
					if (remaining > 0)
						cout << " -> " << ts->getName() << " holds " << e->getName() << " in its grip — its spells are out of reach. (" << remaining << " round(s) left)" << endl;
					else
						cout << " -> The " << ts->getName() << " on " << e->getName() << " wears off. It regains its arcane focus." << endl;
				}
				ts->increaseRound();
				continue;
			}
			
			hps = dynamic_cast<HpStatusEffect*>(se);
			if (hps == nullptr) continue;
			
			e->setCurrentHp(e->getCurrentHp() + hps->getHpAffection());
			
			cout << "==========================================" << endl;
			
			if (hps->getHpAffection() < 0.0f) {
				cout << " -> " << e->getName() << " suffers from " << hps->getName() << ", losing " << -hps->getHpAffection() << " hp." << endl;
			} else {
				cout << " -> " << e->getName() << " is restored by " << hps->getName() << " for " << hps->getHpAffection() << " hp." << endl;
			}
		}
		
		// Process player's status effects
		StatusEffect* se = player->getStatusEffect()[i];
		if (se != nullptr) {
			bool isPlayer = true;
			
			ArmorBreakStatusEffect* abs = dynamic_cast<ArmorBreakStatusEffect*>(se);
			if (abs != nullptr) {
				cout << "==========================================" << endl;
				int remaining = abs->getCurrentDuration() - abs->getCurrentRound() - 1;
				if (remaining > 0)
					cout << " -> " << abs->getName() << " holds — your armor is powerless against the blows. (" << remaining << " round(s) left)" << endl;
				else
					cout << " -> " << abs->getName() << " fades away. Your armor stands firm once more." << endl;
				abs->increaseRound();
				continue;
			}
			
			TauntStatusEffect* ts = dynamic_cast<TauntStatusEffect*>(se);
			if (ts != nullptr) {
				cout << "==========================================" << endl;
				int remaining = ts->getCurrentDuration() - ts->getCurrentRound() - 1;
				if (remaining > 0)
					cout << " -> " << ts->getName() << " burns in your blood — your spells slip from your mind. (" << remaining << " round(s) left)" << endl;
				else
					cout << " -> The " << ts->getName() << " subsides. Your focus returns — spells are yours once more." << endl;
				ts->increaseRound();
				continue;
			}
			
			hps = dynamic_cast<HpStatusEffect*>(se);
			if (hps == nullptr) continue;
			
			player->setCurrentHp(player->getCurrentHp() + hps->getHpAffection());
			
			cout << "==========================================" << endl;
			
			if (hps->getHpAffection() < 0.0f) {
				cout << " -> Dark forces from " << hps->getName() << " bite you, dealing " << -hps->getHpAffection() << " damage." << endl;
			} else {
				cout << " -> A gentle light of " << hps->getName() << " heals you for " << hps->getHpAffection() << " hp." << endl;
			}
		}
	}
	
	this->player->checkStatusEffects();
	for (Enemy* e : enemies)
		e->checkStatusEffects();
}

// ============================================================
//  CHECK DEATHS
// ============================================================
void GroupBattleManager::checkDeaths() {
	if (this->player->getCurrentHp() < 0) this->player->setCurrentHp(0);
	for (Enemy* e : enemies)
		if (e->getCurrentHp() < 0) e->setCurrentHp(0);
	
	if (this->player->getCurrentHp() <= 0) {
		this->isFinished = 1;
		
		cout << "==========================================" << endl;
		cout << " -> You (" << this->player->getName() << ") have fallen..." << endl;
		cout << " -> The darkness swallows you whole." << endl;
		cout << "==========================================" << endl;
		
		system("pause");
		
	} else if (livingEnemies().empty()) {
		this->isFinished = 2;
		
		Player* p = dynamic_cast<Player*>(this->player);
		
		cout << "==========================================" << endl;
		cout << " -> Victory! You (" << p->getName() << ") stand over the fallen." << endl;
		cout << " -> Their life force fuels your ascent." << endl;
		cout << "==========================================" << endl;
		
		giveRewards();
		
		system("pause");
		
	} else {
		this->isFinished = 0;
	}
}

// ============================================================
//  GIVE REWARDS
// ============================================================
void GroupBattleManager::giveRewards() {
	Player* p = dynamic_cast<Player*>(this->player);
	if (p == nullptr) return;
	
	float totalXp = 0;
	bool anyDrop = false;
	
	cout << "==========================================" << endl;
	cout << " -> Spoils of the fight:" << endl;
	
	for (Enemy* e : enemies) {
		totalXp += e->getXpToGet();
		
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
		
		vector<Item*>& items   = e->getRewardItems();
		vector<int>&   chances = e->getRewardChances();
		
		for (size_t i = 0; i < items.size(); i++) {
			int roll = rand() % 100 + 1;
			if (roll <= chances[i]) {
				cout << "     - " << items[i]->getName() << endl;
				p->getInventory().addItem(items[i]);
				anyDrop = true;
			}
		}
	}
	
	if (!anyDrop) cout << "     - Nothing. The creatures carried only spite." << endl;
	
	p->setCurrentXp(p->getCurrentXp() + totalXp);
	cout << " -> Total XP gained: " << totalXp << endl;
	cout << "==========================================" << endl;
}

// ============================================================
//  MAIN BATTLE LOOP
// ============================================================
void GroupBattleManager::battle() {
	int choice;
	srand(time(0));
	system("cls");
	
	while (this->isFinished == 0) {
		int randomiseAttackEnemy = rand() % 10;
		int randomiseSpellEnemy = rand() % 4;
		Player* p = dynamic_cast<Player*>(player);
		
		printBattleState();
		cout << "Your Options:" << endl;
		cout << " 1. Strike with weapon" << endl;
		cout << " 2. Invoke a spell" << endl;
		cout << " 3. Search your pack" << endl;
		cout << "------------------------------------------" << endl;
		cout << "Choose your fate, adventurer: ";
		cin >> choice;
		cout << "==========================================" << endl << endl;
		system("cls");
		
		int fastestEnemySpeed = 0;
		for (Enemy* e : livingEnemies())
			if (e->getSpeed() > fastestEnemySpeed) fastestEnemySpeed = e->getSpeed();
		bool playerFirst = (player->getSpeed() >= fastestEnemySpeed);
		
		bool playerTaunted = false;
		for (int i = 0; i < 4; i++)
			if (dynamic_cast<TauntStatusEffect*>(player->getStatusEffect()[i]) != nullptr) { playerTaunted = true; break; }
		
		switch (choice) {
			case 1: {
				cout << "==========================================" << endl;
				int targetIdx = pickTarget("Who do you strike?");
				Enemy* target = enemies[targetIdx];
				
				if (playerFirst) {
					playerAttack(target);
					cout << endl;
					allEnemiesAct();
				} else {
					allEnemiesAct();
					cout << endl;
					playerAttack(target);
				}
				break;
			}
			
			case 2: {
				if (playerTaunted) {
					cout << "==========================================" << endl;
					cout << " -> The " << player->getName() << " is gripped by the Taunt — the words of your spells dissolve before they form." << endl;
					cout << " -> Your body acts on instinct, hurling itself forward into a basic strike." << endl;
					cout << "==========================================" << endl;
					
					int targetIdx = pickTarget("Who do you strike?");
					if (playerFirst) {
						playerAttack(enemies[targetIdx]);
						cout << endl;
						allEnemiesAct();
					} else {
						allEnemiesAct();
						cout << endl;
						playerAttack(enemies[targetIdx]);
					}
					break;
				}
				
				Spell* s;
				ChainingSpell* cs;
				StatusEffectSpell* sas;
				LifeStealSpell* lss;
				
				cout << "==========================================" << endl;
				printBattleState();
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
				
				int targetIdx = pickTarget("Who is your target?");
				Enemy* target = enemies[targetIdx];
				
				if (playerFirst) {
					playerAttackUsingSpell(target, choice - 1);
					cout << endl;
					allEnemiesAct();
				} else {
					allEnemiesAct();
					cout << endl;
					playerAttackUsingSpell(target, choice - 1);
				}
				
				break;
			}
			
			case 3: {
				Player* p = dynamic_cast<Player*>(player);
				vector<Item*>& items = p->getInventory().getStorage();
				vector<int> usableIndexes;
				
				cout << "==========================================" << endl;
				printBattleState();
				cout << "Provisions in your pack, worn from the road:" << endl << endl;
				
				for (size_t i = 0; i < items.size(); i++) {
					Consumable* c = dynamic_cast<Consumable*>(items[i]);
					AttackGadget* ag = dynamic_cast<AttackGadget*>(items[i]);
					if (c != nullptr) {
						cout << usableIndexes.size() + 1 << ". Consum.   | " << *c << endl;
						usableIndexes.push_back(i);
					} else if (ag != nullptr) {
						cout << usableIndexes.size() + 1 << ". A. Gadget | " << *ag << endl;
						usableIndexes.push_back(i);
					}
				}
				
				if (usableIndexes.empty()) {
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
				
				if (choice <= 0 || choice > (int)usableIndexes.size()) {
					cout << " -> You put it away. The moment to act passes." << endl;
					break;
				}
				
				int realIndex = usableIndexes[choice - 1];
				Consumable* used = dynamic_cast<Consumable*>(items[realIndex]);
				AttackGadget* usedAG = dynamic_cast<AttackGadget*>(items[realIndex]);
				
				if (used != nullptr) {
					float healAmount = used->getHpToRecover();
					player->setCurrentHp(player->getCurrentHp() + healAmount);
					if (player->getCurrentHp() > player->getMaxHp()) player->setCurrentHp(player->getMaxHp());
					cout << "==========================================" << endl;
					cout << " -> You use " << used->getName() << ". " << healAmount << " vitality flows back into your veins." << endl;
					items.erase(items.begin() + realIndex);
				} else if (usedAG != nullptr) {
					int backfireRoll = rand() % 100 + 1;
					cout << "==========================================" << endl;
					string agType = usedAG->getType();
					
					if (backfireRoll <= usedAG->getChanceOfBackfire()) {
						float selfDmg = usedAG->getDamage() * 0.5;
						player->setCurrentHp(player->getCurrentHp() - selfDmg);
						cout << " -> You hurl " << usedAG->getName() << " — but it detonates in your hand." << endl;
						cout << " -> The blast tears back into you for " << selfDmg << " damage." << endl;
					} else {
						player->setCurrentHp(player->getCurrentHp() - usedAG->getDamage());
						cout << " -> You hurl " << usedAG->getName() << " at " << choice << "." << endl;
						cout << " -> It connects. " << usedAG->getDamage() << " damage tears through them." << endl;
					}
					items.erase(items.begin() + realIndex);
				} else {
					cout << " -> That item cannot be used." << endl;
				}
				
				cout << endl;
				allEnemiesAct();
				break;
			}	
			
		default:
			break;
		}
		
		roundIncrement();
		checkStatusEffects();
		checkDeaths();
		
		if (this->isFinished == 0) system("cls");
	}
}
