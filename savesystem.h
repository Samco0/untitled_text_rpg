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
#include "savesystem.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
using namespace std;

void saveSpell(std::ofstream& file, Spell* spell);
void saveGame(int mapIndex, Player& player, int slot);
Spell* loadSpell(std::ifstream& file);
bool loadGame(int& mapIndex, Player& player, int slot);

#endif
