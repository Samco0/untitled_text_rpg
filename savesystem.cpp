#include "savesystem.h"
#include "spell.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "item.h"
#include "weapon.h"
#include "armor.h"
#include "scroll.h"
#include "consumable.h"
#include <fstream>
#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

static string slotFile(int slot){ return "slot" + to_string(slot) + "_save.txt"; }

// ============================================================
//  SPELL  save / load
// ============================================================
void saveSpell(ofstream& f, Spell* spell){
	if (!spell){ f << -1 << "\n"; return; }
	
	if      (dynamic_cast<LifeStealSpell*>(spell))    f << 3 << "\n";
	else if (dynamic_cast<ChainingSpell*>(spell))     f << 1 << "\n";
	else if (dynamic_cast<StatusEffectSpell*>(spell)) f << 2 << "\n";
	else                                               f << 0 << "\n";
	
	f << spell->getName()            << "\n";
	f << spell->getDescription()     << "\n";
	f << spell->getDmg()             << "\n";
	f << spell->getFullCooldown()    << "\n";
	f << spell->getRemainingCooldown()<< "\n";
	
	if (ChainingSpell* cs = dynamic_cast<ChainingSpell*>(spell)){
		f << cs->getMinHits() << "\n";
		f << cs->getMaxHits() << "\n";
	}
	if (StatusEffectSpell* ss = dynamic_cast<StatusEffectSpell*>(spell)){
		f << ss->getChanceToRecieve() << "\n";
		StatusEffect* st = ss->getStatusToGive();
		if (dynamic_cast<HpStatusEffect*>(st)) f << 1 << "\n";
		else                                    f << 0 << "\n";
		f << st->getName()            << "\n";
		f << st->getMinDuration()     << "\n";
		f << st->getMaxDuration()     << "\n";
		f << st->getCurrentDuration() << "\n";
		f << st->getCurrentRound()    << "\n";
		if (HpStatusEffect* hp = dynamic_cast<HpStatusEffect*>(st))
			f << hp->getHpAffection() << "\n";
	}
	if (LifeStealSpell* ls = dynamic_cast<LifeStealSpell*>(spell))
		f << ls->getLifeStealRate() << "\n";
}

Spell* loadSpell(ifstream& f){
	int type; f >> type; f.ignore();
	if (type == -1) return nullptr;
	
	string name, desc;
	getline(f, name);
	getline(f, desc);
	
	float dmg; int fullCd, remCd;
	f >> dmg >> fullCd >> remCd; f.ignore();
	
	Spell* spell = nullptr;
	
	if (type == 0){
		spell = new Spell(name, desc, dmg, fullCd);
	} else if (type == 1){
		int minH, maxH; f >> minH >> maxH; f.ignore();
		spell = new ChainingSpell(name, desc, dmg, fullCd, minH, maxH);
	} else if (type == 2){
		int chance, stType; f >> chance >> stType; f.ignore();
		string stName; getline(f, stName);
		int minD, maxD, curD, curR; f >> minD >> maxD >> curD >> curR;
		StatusEffect* st = nullptr;
		if (stType == 1){
			float hpAff; f >> hpAff; f.ignore();
			st = new HpStatusEffect(stName, maxD, minD, hpAff);
		} else {
			f.ignore();
			st = new StatusEffect(stName, maxD, minD);
		}
		st->setCurrentDuration(curD);
		st->setCurrentRound(curR);
		spell = new StatusEffectSpell(name, desc, dmg, fullCd, chance, st);
	} else if (type == 3){
		int ls; f >> ls; f.ignore();
		spell = new LifeStealSpell(name, desc, dmg, fullCd, ls);
	}
	
	if (spell) spell->setRemainingCooldown(remCd);
	return spell;
}

// ============================================================
//  ARMOR slot  save / load  (0 = not equipped)
// ============================================================
static void saveArmorSlot(ofstream& f, Armor* a){
	if (!a){ f << 0 << "\n"; return; }
	f << 1 << "\n";
	f << a->getName()            << "\n";
	f << a->getDescription()     << "\n";
	f << a->getDamageReduction() << "\n";
	f << a->getType()            << "\n";
}
static Armor* loadArmorSlot(ifstream& f){
	int present; f >> present; f.ignore();
	if (!present) return nullptr;
	string name, desc; getline(f, name); getline(f, desc);
	float dr; int type; f >> dr >> type; f.ignore();
	return new Armor(name, desc, dr, type);
}

// ============================================================
//  WEAPON  save / load
// ============================================================
static void saveWeapon(ofstream& f, Weapon* w){
	if (!w){ f << 0 << "\n"; return; }
	f << 1 << "\n";
	f << w->getName()        << "\n";
	f << w->getType()        << "\n";
	f << w->getDescription() << "\n";
	f << w->getDmg()         << "\n";
}
static Weapon* loadWeapon(ifstream& f){
	int present; f >> present; f.ignore();
	if (!present) return nullptr;
	string name, wtype, desc; getline(f, name); getline(f, wtype); getline(f, desc);
	float dmg; f >> dmg; f.ignore();
	return new Weapon(name, wtype, desc, dmg);
}

// ============================================================
//  INVENTORY  save / load
// ============================================================
static void saveInventory(ofstream& f, Player& player){
	vector<Item*>& storage = player.getInventory().getStorage();
	f << storage.size() << "\n";
	for (Item* item : storage){
		if (Weapon* w = dynamic_cast<Weapon*>(item)){
			f << 1 << "\n";
			f << w->getName()        << "\n";
			f << w->getType()        << "\n";
			f << w->getDescription() << "\n";
			f << w->getDmg()         << "\n";
		} else if (Armor* a = dynamic_cast<Armor*>(item)){
			f << 2 << "\n";
			f << a->getName()            << "\n";
			f << a->getDescription()     << "\n";
			f << a->getDamageReduction() << "\n";
			f << a->getType()            << "\n";
		} else if (Scroll* s = dynamic_cast<Scroll*>(item)){
			f << 3 << "\n";
			f << s->getName()        << "\n";
			f << s->getDescription() << "\n";
			saveSpell(f, s->getHeldSpell());
		} else if (Consumable* c = dynamic_cast<Consumable*>(item)){
			f << 4 << "\n";
			f << c->getName()        << "\n";
			f << c->getDescription() << "\n";
			f << c->getHpToRecover() << "\n";
		}
	}
}

static void loadInventory(ifstream& f, Player& player){
	player.getInventory().getStorage().clear();
	int size; f >> size; f.ignore();
	for (int i = 0; i < size; i++){
		int type; f >> type; f.ignore();
		string name, desc;
		getline(f, name); getline(f, desc);
		if (type == 1){
			string wtype; getline(f, wtype); // save order: name, wtype, desc, dmg
			getline(f, desc);
			float dmg; f >> dmg; f.ignore();
			player.getInventory().addItem(new Weapon(name, wtype, desc, dmg));
		} else if (type == 2){
			float dr; int atype; f >> dr >> atype; f.ignore();
			player.getInventory().addItem(new Armor(name, desc, dr, atype));
		} else if (type == 3){
			// name is auto-derived from spell in Scroll constructor — only desc was saved
			Spell* sp = loadSpell(f);
			player.getInventory().addItem(new Scroll(desc, sp));
		} else if (type == 4){
			float hp; f >> hp; f.ignore();
			player.getInventory().addItem(new Consumable(name, desc, hp));
		}
	}
}

// ============================================================
//  FULL SAVE  — single file, every piece of player state
// ============================================================
void saveGame(int mapIndex, Player& player, int slot, const string& locationLabel){
	ofstream f(slotFile(slot));
	if (!f.is_open()) return;
	
	// --- header (readable by getSaveInfo without full parse) ---
	f << player.getName()      << "\n";   // line 1
	f << player.getLevel()     << "\n";   // line 2
	f << player.getSoulStones()<< "\n";   // line 3
	f << mapIndex              << "\n";   // line 4
	f << locationLabel         << "\n";   // line 5
	
	// --- full stats ---
	f << player.getMaxHp()     << "\n";
	f << player.getCurrentHp() << "\n";
	f << player.getDmg()       << "\n";
	f << player.getSpeed()     << "\n";
	f << player.getCritChance()<< "\n";
	f << player.getCritValue() << "\n";
	f << player.getCurrentXp() << "\n";
	f << player.getRequireXp() << "\n";
	
	// --- equipped weapon ---
	saveWeapon(f, player.getWeapon());
	
	// --- 4 known spells ---
	for (int i = 0; i < 4; i++) saveSpell(f, player.getSpells()[i]);
	
	// --- 5 armor slots ---
	saveArmorSlot(f, player.getHelmet());
	saveArmorSlot(f, player.getChestplate());
	saveArmorSlot(f, player.getGloves());
	saveArmorSlot(f, player.getLeggings());
	saveArmorSlot(f, player.getBoots());
	
	// --- inventory ---
	saveInventory(f, player);
	
	f.close();
}

// ============================================================
//  FULL LOAD
// ============================================================
bool loadGame(int& mapIndex, Player& player, int slot){
	ifstream f(slotFile(slot));
	if (!f.is_open()) return false;
	
	// header
	string name; getline(f, name); player.setName(name);
	int level, soulStones; f >> level >> soulStones; f.ignore();
	f >> mapIndex; f.ignore();
	string locationLabel; getline(f, locationLabel); // consume label line — must not skip
	
	// stats
	float maxHp, curHp, dmg, critVal, curXp, reqXp;
	int speed, critChance;
	f >> maxHp >> curHp >> dmg >> speed >> critChance >> critVal >> curXp >> reqXp;
	f.ignore();
	
	player.setMaxHp(maxHp);
	player.setCurrentHp(curHp);
	player.setDmg(dmg);
	player.setSpeed(speed);
	player.setCritChance(critChance);
	player.setCritValue(critVal);
	// restore level/xp without triggering level-up loop
	player.setLevel(level);
	player.setRequireXp(reqXp);
	// setCurrentXp is additive (does +=), so zero it out first then add saved value
	player.setCurrentXp(-player.getCurrentXp()); // zero
	player.setCurrentXp(curXp);                  // restore exact amount
	
	player.setSoulStones(soulStones);
	
	// weapon
	Weapon* w = loadWeapon(f);
	if (w) player.setWeapon(w);
	
	// spells
	for (int i = 0; i < 4; i++){
		Spell* sp = loadSpell(f);
		player.setSpell(i, sp);
	}
	
	// armor
	Armor* helmet     = loadArmorSlot(f); if (helmet)     player.setHelmet(helmet);
	Armor* chest      = loadArmorSlot(f); if (chest)      player.setChestplate(chest);
	Armor* gloves     = loadArmorSlot(f); if (gloves)     player.setGloves(gloves);
	Armor* leggings   = loadArmorSlot(f); if (leggings)   player.setLeggings(leggings);
	Armor* boots      = loadArmorSlot(f); if (boots)      player.setBoots(boots);
	
	// inventory
	loadInventory(f, player);
	
	f.close();
	return true;
}

// ============================================================
//  DELETE SAVE
// ============================================================
void deleteSave(int slot){
	string path = slotFile(slot);
	if (fs::exists(path)) fs::remove(path);
}

// ============================================================
//  QUICK SUMMARY  (reads only the 4-line header)
// ============================================================
SaveInfo getSaveInfo(int slot){
	SaveInfo info;
	ifstream f(slotFile(slot));
	if (!f.is_open()){ info.exists = false; return info; }
	info.exists = true;
	getline(f, info.playerName);
	f >> info.level >> info.soulStones >> info.mapIndex;
	f.ignore();
	getline(f, info.locationLabel);
	f.close();
	return info;
}

// ============================================================
//  SCAN FOR EXISTING SLOTS
// ============================================================
vector<int> getExistingSlots(){
	vector<int> slots;
	for (int i = 1; i <= 9; i++){
		if (fs::exists(slotFile(i))) slots.push_back(i);
	}
	return slots;
}
