#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <windows.h>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "map.h"
#include "weapon.h"
#include "savesystem.h"
#include "data_snailsville.h"
#include "data_rustheap.h"

using namespace std;

// =============================================
//  TERMINAL COLOR HELPERS
// =============================================
static void setMapColor(const string& mapName) {
	if (mapName == "The Snailsville")
		cout << "\033[32m";
	else if (mapName == "The Rustheap")
		cout << "\033[33m";
}

static void resetColor() {
	cout << "\033[90m";
}

int main() {
	srand(time(NULL));
	SetConsoleTitle("Untitled Text RPG by Samqo");
	
	int slot = 1;
	int currentMapIndex = 0;  // room within map
	int currentMapIdx = 0;    // which map
	string playerName;
	
	// Player starting weapons and spells
	// These live in main — no heap allocation needed for a handful of starting objects
	Spell sp1("Trembling Blow", "A weak, unsteady punch that barely fazes the target.", 3, 1, "Physical");
	Spell sp2("Cursed Kick", "A timid kick, shadowed by a faint curse, barely affects the enemy.", 2.5, 1, "Dark");
	Spell* s1[4] = { &sp1, &sp2, nullptr, nullptr };
	Weapon w1("Scorched Splinter", "Sword", "A short blade of charred wood, brittle and blackened. It should have been thrown away long ago.", 1.5);
	
	Player player("", 25, 1.5, 0, 4, 15, 1.5, &w1, s1);
	
	// =============================================
	//  START MENU
	// =============================================
	while (true) {
		system("cls");
		cout << "\033[90m";
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "            untitled text rpg                 " << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "  1. Step into Shadows   (New Game)"           << endl;
		cout << "  2. Recall Fallen       (Load Game)"          << endl;
		cout << "  3. Burn Memories       (Delete Save)"        << endl;
		cout << "  4. Fade Away           (Exit)"               << endl;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << "Your choice: ";
		
		int choice; cin >> choice;
		
		// ---- NEW GAME ----
		if (choice == 1) {
			system("cls");
			
			vector<int> existing = getExistingSlots();
			
			cout << "===============================================" << endl;
			cout << " Choose a grave to claim (1-9):              " << endl;
			cout << "-----------------------------------------------" << endl;
			for (int i = 1; i <= 9; i++) {
				SaveInfo info = getSaveInfo(i);
				cout << "  " << i << ". ";
				if (info.exists)
					cout << "[OCCUPIED] " << info.playerName
					<< "  |  level " << info.level
					<< "  |  stones " << info.soulStones
					<< "  |  " << (info.locationLabel.empty() ? "room " + to_string(info.mapIndex) : info.locationLabel);
				else
					cout << "[empty]";
				cout << endl;
			}
			cout << "===============================================" << endl;
			cout << "Slot: ";
			int slotChoice; cin >> slotChoice;
			if (slotChoice < 1 || slotChoice > 9) {
				cout << "Invalid slot. Returning..." << endl;
				system("pause");
				continue;
			}
			slot = slotChoice;
			
			system("cls");
			cout << "Whisper your name to the void: ";
			cin >> playerName;
			player.setName(playerName);
			currentMapIndex = 0;
			
			system("cls");
			cout << "Very well, " << player.getName()
			<< ".\nYou tread into the shadowed path, heart pounding...\n";
			system("pause");
			break;
			
			// ---- LOAD GAME ----
		} else if (choice == 2) {
			system("cls");
			vector<int> existing = getExistingSlots();
			if (existing.empty()) {
				cout << "No graves found. The shadows are empty." << endl;
				system("pause");
				continue;
			}
			
			cout << "===============================================" << endl;
			cout << " Echoes of past adventurers linger:          " << endl;
			cout << "-----------------------------------------------" << endl;
			for (int s : existing) {
				SaveInfo info = getSaveInfo(s);
				cout << "  " << s << ". " << info.playerName << endl;
				cout << "     level "       << info.level
				<< "  |  Soul Stones " << info.soulStones
				<< "  |  " << (info.locationLabel.empty() ? "room " + to_string(info.mapIndex) : info.locationLabel) << endl;
				cout << endl;
			}
			cout << "===============================================" << endl;
			cout << "Choose a grave to awaken (or 0 to go back): ";
			int loadChoice; cin >> loadChoice;
			
			if (loadChoice == 0) continue;
			
			slot = loadChoice;
			if (loadGame(currentMapIndex, currentMapIdx, player, slot)) {
				system("cls");
				cout << "Something stirs..." << endl;
				cout << player.getName() << " returns from the dark." << endl;
				system("pause");
				break;
			} else {
				cout << "Nothing there." << endl;
				system("pause");
			}
			
			// ---- DELETE SAVE ----
		} else if (choice == 3) {
			system("cls");
			vector<int> existing = getExistingSlots();
			if (existing.empty()) {
				cout << "No graves to erase." << endl;
				system("pause");
				continue;
			}
			
			cout << "===============================================" << endl;
			cout << " Which grave shall be erased?                " << endl;
			cout << "-----------------------------------------------" << endl;
			for (int s : existing) {
				SaveInfo info = getSaveInfo(s);
				cout << "  " << s << ". " << info.playerName
				<< "  (level " << info.level << ")" << endl;
			}
			cout << "===============================================" << endl;
			cout << "Slot (0 to cancel): ";
			int delChoice; cin >> delChoice;
			
			if (delChoice == 0) {
				cout << "Returning..." << endl;
				system("pause");
				continue;
			}
			
			SaveInfo info = getSaveInfo(delChoice);
			if (!info.exists) {
				cout << "That grave is already empty." << endl;
			} else {
				cout << "Erase " << info.playerName << " forever? (1 = yes): ";
				int confirm; cin >> confirm;
				if (confirm == 1) {
					deleteSave(delChoice);
					cout << "The name fades. " << info.playerName << " is forgotten." << endl;
				} else {
					cout << "Their memory endures... for now." << endl;
				}
			}
			system("pause");
			
			// ---- EXIT ----
		} else if (choice == 4) {
			cout << "You vanish into the void..." << endl;
			return 0;
		} else {
			cout << "No path for that choice." << endl;
			system("pause");
		}
	}
	
	// Save starting state
	saveGame(currentMapIndex, currentMapIdx, player, slot);
	
	// =============================================
	//  BUILD WORLD DATA
	//  All enemies, spells, items owned here.
	//  Automatically freed when main() returns.
	// =============================================
	SnailsvilleData snailsvilleData = buildSnailsville();
	RustheapData    rustheapData    = buildRustheap();
	
	// =============================================
	//  MAP INITIALIZATION
	// =============================================
	Map snailsville("The Snailsville", &player);
	Map rustheap("The Rustheap", &player);
	
	snailsville.generateMap(
		snailsvilleData.easy,
		snailsvilleData.medium,
		snailsvilleData.hard,
		snailsvilleData.boss,
		5, 4, 3
		);
	
	rustheap.generateMap(
		rustheapData.easy,
		rustheapData.medium,
		rustheapData.hard,
		rustheapData.boss,
		9, 7, 5
		);
	
	vector<Map*> maps = { &snailsville, &rustheap };
	
	maps[currentMapIdx]->setPlayerPosition(currentMapIndex);
	
	// =============================================
	//  MAIN GAME LOOP
	// =============================================
	while (true) {
		Map* currentMap = maps[currentMapIdx];
		setMapColor(currentMap->getName());
		
		{
			string label = currentMap->getName() + " - " + currentMap->getLocations()[currentMap->getPlayerPosition()]->getName();
			saveGame(currentMap->getPlayerPosition(), currentMapIdx, player, slot, label);
		}
		
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << player;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		
		currentMap->movePlayer();
		
		// ---- DEATH HANDLING ----
		if (player.getCurrentHp() <= 0) {
			resetColor();
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "   The world goes dark around you..." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			
			if (player.useSoulStone()) {
				int savedRoom, savedMapIdx;
				loadGame(savedRoom, savedMapIdx, player, slot);
				currentMapIdx = savedMapIdx;
				currentMap->setPlayerPosition(savedRoom);
				
				{
					string label = currentMap->getName() + " - " + currentMap->getLocations()[savedRoom]->getName();
					saveGame(savedRoom, savedMapIdx, player, slot, label);
				}
				
				cout << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				cout << " -> A Soul Stone shatters in your pocket." << endl;
				cout << " -> Warmth floods back. Your wounds seal." << endl;
				cout << " -> You stand again, just before the threshold." << endl;
				cout << " -> Soul Stones remaining: ";
				for (int i = 0; i < player.getSoulStones(); i++) cout << "[*] ";
				if (player.getSoulStones() == 0) cout << "none";
				cout << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				system("pause");
				continue;
			} else {
				cout << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				cout << " -> No Soul Stones remain." << endl;
				cout << " -> The darkness claims you completely." << endl;
				cout << " -> " << player.getName() << "'s journey ends here." << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				resetColor();
				deleteSave(slot);
				system("pause");
				break;
			}
		}
		
		// ---- MAP COMPLETION ----
		if (currentMap->getPlayerFinished()) {
			currentMapIdx++;
			
			if (currentMapIdx >= (int)maps.size()) {
				resetColor();
				system("cls");
				cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				cout << " You have survived everything." << endl;
				cout << " " << player.getName() << " steps out of the dark for good." << endl;
				cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				deleteSave(slot);
				system("pause");
				break;
			}
			
			resetColor();
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << " " << currentMap->getName() << " falls behind you." << endl;
			cout << " The road ahead leads to " << maps[currentMapIdx]->getName() << "." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			maps[currentMapIdx]->setPlayerPosition(0);
			saveGame(0, currentMapIdx, player, slot,
				maps[currentMapIdx]->getName() + " - " +
				maps[currentMapIdx]->getLocations()[0]->getName());
			system("pause");
		}
	}
	
	// SnailsvilleData and RustheapData go out of scope here.
	// All unique_ptrs are automatically destroyed — no leaks.
	return 0;
}
