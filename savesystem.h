#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "player.h"
#include "weapon.h"
#include "armor.h"
#include "consumable.h"
#include "scroll.h"
#include "attackgadget.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
using namespace std;

// ---- Spell helpers ----
void   saveSpell(ofstream& file, Spell* spell);
Spell* loadSpell(ifstream& file);

// ---- Full game save/load ----
// Everything goes into a single slot<N>_save.txt file
void saveGame(int roomIndex, int mapIdx, Player& player, int slot, const string& locationLabel = "");
bool loadGame(int& roomIndex, int& mapIdx, Player& player, int slot);

// ---- Delete a save slot (removes the file) ----
void deleteSave(int slot);

// ---- Quick summary for the start menu ----
struct SaveInfo {
	bool   exists         = false;
	string playerName     = "";
	int    level          = 0;
	int    soulStones     = 0;
	int    mapIndex       = 0;
	string locationLabel  = "";
};
SaveInfo getSaveInfo(int slot);

// ---- Scan for existing save files and return their slot numbers ----
vector<int> getExistingSlots();

#endif
