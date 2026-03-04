#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <fstream>
#include <iostream>
#include <vector>
#include "player.h"
#include "weapon.h"
#include "armor.h"
#include "consumable.h"
#include "scroll.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
using namespace std;

// Functions to save and load spells/games
void saveSpell(ofstream& file, Spell* spell);
Spell* loadSpell(ifstream& file);

void saveGame(int mapIndex, Player& player, int slot);
bool loadGame(int& mapIndex, Player& player, int slot);

#endif
