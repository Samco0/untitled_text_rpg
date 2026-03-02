#include "savesystem.h"

// --- uloží jeden spell do souboru ---
void saveSpell(ofstream& file, Spell* spell) {
	if (dynamic_cast<ChainingSpell*>(spell))
		file << 1 << "\n";
	else if (dynamic_cast<StatusEffectSpell*>(spell))
		file << 2 << "\n";
	else
		file << 0 << "\n";
	
	file << spell->getName() << "\n";
	file << spell->getDescription() << "\n";
	file << spell->getDmg() << "\n";
	file << spell->getFullCooldown() << "\n";
	file << spell->getRemainingCooldown() << "\n";
	
	if (ChainingSpell* cs = dynamic_cast<ChainingSpell*>(spell)) {
		file << cs->getMinHits() << "\n";
		file << cs->getMaxHits() << "\n";
	}
	
	if (StatusEffectSpell* ss = dynamic_cast<StatusEffectSpell*>(spell)) {
		file << ss->getChanceToRecieve() << "\n";
		
		StatusEffect* status = ss->getStatusToGive();
		if (dynamic_cast<HpStatusEffect*>(status))
			file << 1 << "\n";
		else
			file << 0 << "\n";
		
		file << status->getName() << "\n";
		file << status->getMinDuration() << "\n";
		file << status->getMaxDuration() << "\n";
		file << status->getCurrentDuration() << "\n";
		file << status->getCurrentRound() << "\n";
		
		if (HpStatusEffect* hp = dynamic_cast<HpStatusEffect*>(status)) {
			file << hp->getHpAffection() << "\n";
		}
	}
}

// --- uloží všechny položky hráče ---
void saveItems(Player& player, int slot) {
	ofstream file("slot" + to_string(slot) + "_items.txt");
	if (!file.is_open()) {
		cout << "Chyba pri ukladani itemu!\n";
		return;
	}
	
	vector<Item*>& storage = player.getInventory().getStorage();
	file << storage.size() << "\n";
	
	for (Item* item : storage) {
		if (Weapon* w = dynamic_cast<Weapon*>(item)) {
			file << 1 << "\n";
			file << w->getName() << "\n";
			file << w->getDescription() << "\n";
			file << w->getDmg() << "\n";
			file << w->getType() << "\n";
		} else if (Armor* a = dynamic_cast<Armor*>(item)) {
			file << 2 << "\n";
			file << a->getName() << "\n";
			file << a->getDescription() << "\n";
			file << a->getDamageReduction() << "\n";
			file << a->getType() << "\n";
		} else if (Scroll* s = dynamic_cast<Scroll*>(item)) {
			file << 3 << "\n";
			file << s->getName() << "\n";
			file << s->getDescription() << "\n";
			saveSpell(file, s->getHeldSpell());
		} else if (Consumable* c = dynamic_cast<Consumable*>(item)) {
			file << 4 << "\n";
			file << c->getName() << "\n";
			file << c->getDescription() << "\n";
			file << c->getHpToRecover() << "\n";
		}
	}
	
	file.close();
}

// --- načte jeden spell ---
Spell* loadSpell(ifstream& file) {
	int spellType;
	file >> spellType;
	file.ignore();
	
	string name, description;
	getline(file, name);
	getline(file, description);
	
	float dmg;
	int fullCd, remainingCd;
	file >> dmg >> fullCd >> remainingCd;
	file.ignore();
	
	Spell* spell = nullptr;
	
	if (spellType == 0) {
		spell = new Spell(name, description, dmg, fullCd);
		spell->setRemainingCooldown(remainingCd);
	} else if (spellType == 1) {
		int minHits, maxHits;
		file >> minHits >> maxHits;
		file.ignore();
		spell = new ChainingSpell(name, description, dmg, fullCd, minHits, maxHits);
		spell->setRemainingCooldown(remainingCd);
	} else if (spellType == 2) {
		int chance;
		file >> chance;
		int statusType;
		file >> statusType;
		file.ignore();
		
		string statusName;
		getline(file, statusName);
		
		int minDur, maxDur, currDur, currRound;
		file >> minDur >> maxDur >> currDur >> currRound;
		
		StatusEffect* status = nullptr;
		if (statusType == 0) {
			status = new StatusEffect(statusName, maxDur, minDur);
		} else if (statusType == 1) {
			float hpAff;
			file >> hpAff;
			status = new HpStatusEffect(statusName, maxDur, minDur, hpAff);
		}
		
		status->setCurrentDuration(currDur);
		status->setCurrentRound(currRound);
		
		spell = new StatusEffectSpell(name, description, dmg, fullCd, chance, status);
		spell->setRemainingCooldown(remainingCd);
	}
	
	return spell;
}

// --- načte všechny položky hráče ---
void loadItems(Player& player, int slot) {
	ifstream file("slot" + to_string(slot) + "_items.txt");
	if (!file.is_open()) {
		cout << "Item save neexistuje.\n";
		return;
	}
	
	player.getInventory().getStorage().clear();
	
	int size;
	file >> size;
	file.ignore();
	
	for (int i = 0; i < size; i++) {
		int type;
		file >> type;
		file.ignore();
		
		string name, description;
		getline(file, name);
		getline(file, description);
		
		if (type == 1) {
			float dmg;
			string wType;
			file >> dmg >> wType;
			file.ignore();
			player.getInventory().addItem(new Weapon(name, wType, description, dmg));
		} else if (type == 2) {
			float dr;
			int aType;
			file >> dr >> aType;
			file.ignore();
			player.getInventory().addItem(new Armor(name, description, dr, aType));
		} else if (type == 3) {
			Spell* spell = loadSpell(file);
			player.getInventory().addItem(new Scroll(description, spell));
		} else if (type == 4) {
			int hp;
			file >> hp;
			file.ignore();
			player.getInventory().addItem(new Consumable(name, description, hp));
		}
	}
	
	file.close();
}

// --- uloží a načte celou hru ---
void saveGame(int mapIndex, Player& player, int slot) {
	ofstream file("slot" + to_string(slot) + "_map.txt");
	if (file.is_open()) {
		file << mapIndex << "\n";
		file.close();
	}
	saveItems(player, slot);
}

bool loadGame(int& mapIndex, Player& player, int slot) {
	ifstream file("slot" + to_string(slot) + "_map.txt");
	if (!file.is_open()) return false;
	
	file >> mapIndex;
	file.close();
	loadItems(player, slot);
	return true;
}
