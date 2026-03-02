#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <filesystem>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "map.h"
#include "weapon.h"
#include "savesystem.h"

using namespace std;
namespace fs = std::filesystem;

int main() {
	srand(time(NULL));
	
	int slot = 1;
	int currentMapIndex = 0;
	bool newGame = false;
	
	//Player start up weapons and spells
	Spell sp1("Trembling Blow", "A weak, unsteady punch that barely fazes the target.", 3, 1);
	Spell sp2("Cursed Kick", "A timid kick, shadowed by a faint curse, barely affects the enemy.", 2.5, 1);
	Spell* s1[4] = { &sp1, &sp2, nullptr, nullptr};
	Weapon w1("Charred Wooden Tanto", "Sword", "A simple wooden tanto, slightly burnt and brittle, whispering faint dark secrets.", 1.5);
	
	//The player themselves
	Player p1("", 30, 1.5, 0, 4, 20, 1.2, &w1, s1);
	
	// --- START MENU ---
	// --- START MENU (Dark Fantasy Minimal) ---
	while (true) {
		cout << "========================\n";
		cout << "       DARK BEGINNING   \n";
		cout << "========================\n";
		cout << "1. Step into Shadows (New Game)\n";
		cout << "2. Recall Fallen (Load Game)\n";
		cout << "3. Burn Memories (Delete Save)\n";
		cout << "4. Fade Away (Exit)\n";
		cout << "Your choice: ";
		
		int choice;
		cin >> choice;
		
		if (choice == 1) {
			cout << "You step into the gloom, trembling...\n";
			newGame = true;
			currentMapIndex = 0;
			break;
			
		} else if (choice == 2) {
			cout << "The echoes of past adventurers linger:\n";
			for (int i = 1; i <= 3; i++) {
				ifstream f("slot" + to_string(i) + "_map.txt");
				cout << i << ". Grave " << i;
				if (f.is_open()) cout << " (contains traces)";
				cout << "\n";
			}
			
			cout << "Choose a grave to awaken (1-3), any other key to step back: ";
			int loadChoice;
			cin >> loadChoice;
			
			if (loadChoice >= 1 && loadChoice <= 3) {
				slot = loadChoice;
				if (loadGame(currentMapIndex, p1, slot)) {
					cout << "Something stirs within Grave " << slot << "...\n";
					newGame = false;
					break;
				} else {
					cout << "Empty... nothing remains.\n";
				}
			} else {
				cout << "You retreat into the shadows...\n";
			}
			
		} else if (choice == 3) {
			cout << "Select grave to erase (1-3), any other key to cancel: ";
			int delChoice;
			cin >> delChoice;
			
			if (delChoice >= 1 && delChoice <= 3) {
				string mapFile = "slot" + to_string(delChoice) + "_map.txt";
				string itemsFile = "slot" + to_string(delChoice) + "_items.txt";
				
				bool deleted = false;
				if (fs::exists(mapFile)) { fs::remove(mapFile); deleted = true; }
				if (fs::exists(itemsFile)) { fs::remove(itemsFile); deleted = true; }
				
				if (deleted)
					cout << "Grave " << delChoice << " has been erased from memory.\n";
				else
					cout << "Nothing to erase in Grave " << delChoice << ".\n";
			} else {
				cout << "The urge passes. Returning...\n";
			}
			
		} else if (choice == 4) {
			cout << "You vanish into the void...\n";
			return 0;
			
		} else {
			cout << "No path for that choice. Try again.\n";
		}
		
		// Clear screen after iteration
		system("cls");
	}
	
	//When you create a new game, this is the part where it is created
	Armor helmet1("Helmet of the damned", "That damn helmet", 20, 1);
	Consumable cons1("Bandage", "A piece of cloth used to wrap wounds.", 25);
	
	if (newGame) {
		p1.getInventory().addItem(&helmet1);
		p1.getInventory().addItem(&cons1);
		saveGame(currentMapIndex, p1, slot); //initial save
	}
	
	// --- ENEMIES + MAPS ---
	Spell se1("Gob-ball!", "A ball of liquid gold.", 5, 3);
	Spell* s2[4] = { &se1, nullptr, nullptr, nullptr };
	
	// --- Enemy 1 ---
	Enemy e1("Goblin 1", 15, 1, 1, 1, 5, 10, 3, s2);
	CombatLocation cl1("Street 1", &e1);
	vector<Location*> loc1 = { &cl1 };
	Map map1("Map One", &p1, loc1);
	
	// --- Enemy 1 rewards ---
	Consumable apple("Apple", "Restores 15 HP", 15);
	Consumable melon("Melon", "Restores 30 HP", 30);
	Spell spell_scroll1("Hocus Muccus", "Classic attack spell", 20, 4);
	Scroll scroll1("Scroll containing the spell Hocus Muccus", &spell_scroll1);
	Weapon shellsmasher("Shellsmasher", "Weapon", "A heavy smashing weapon", 10);
	
	e1.addReward(&apple, 60);
	e1.addReward(&melon, 20);
	e1.addReward(&scroll1, 15);
	e1.addReward(&shellsmasher, 5);
	
	// --- Enemy 2 ---
	Enemy e2("Goblin Boss", 100, 2, 1, 2, 4, 12, 2, s2);
	CombatLocation cl2("Boss Room", &e2);
	vector<Location*> loc2 = { &cl2 };
	Map map2("Map Two", &p1, loc2);
	
	// --- Enemy 2 rewards ---
	Consumable megaPotion("Mega Potion", "Restores 100 HP", 100);
	Weapon doomBlade("Doom Blade", "Sword", "A legendary sword of doom", 25);
	Spell fireball("Fireball", "A fiery explosion", 40, 3);
	Scroll scroll_fireball("Scroll containing the spell Fireball", &fireball);
	
	e2.addReward(&megaPotion, 50);
	e2.addReward(&doomBlade, 30);
	e2.addReward(&scroll_fireball, 20);
	
	// --- maps vector ---
	vector<Map*> maps = { &map1, &map2 };
	
	// --- GAME LOOP ---
	while (currentMapIndex < maps.size()) {
		Map* currentMap = maps[currentMapIndex];
		
		while (!currentMap->getPlayerFinished()) {
			cout << *currentMap;
			currentMap->movePlayer();
			
			if (p1.getCurrentHp() <= 0) {
				cout << "\nYOU DIED. Loading last checkpoint...\n";
				if (loadGame(currentMapIndex, p1, slot)) {
					cout << "Checkpoint loaded!\n";
				} else {
					cout << "No saved checkpoint found. Restarting map.\n";
				}
				break;
			}
		}
		
		if (p1.getCurrentHp() > 0) {
			cout << "\nMap completed. Saving progress...\n";
			saveGame(currentMapIndex, p1, slot);
			currentMapIndex++;
		}
	}
	
	cout << "\nGAME COMPLETED.\n";
	return 0;
}
