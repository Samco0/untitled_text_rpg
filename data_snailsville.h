#ifndef DATA_SNAILSVILLE_H
#define DATA_SNAILSVILLE_H
#pragma once
#include <vector>
#include <memory>
#include "map.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "weapon.h"

// SnailsvilleData owns every object created for Snailsville.
// All Location* pointers in easy/medium/hard/boss point into 'locations'.
// Keep this struct alive for as long as the Map is in use.
struct SnailsvilleData {
	std::vector<std::unique_ptr<Spell>>                  spells;
	std::vector<std::unique_ptr<HpStatusEffect>>         hpEffects;
	std::vector<std::unique_ptr<ArmorBreakStatusEffect>> armorEffects;
	std::vector<std::unique_ptr<TauntStatusEffect>>      tauntEffects;
	std::vector<std::unique_ptr<Enemy>>                  enemies;
	std::vector<std::unique_ptr<Consumable>>             consumables;
	std::vector<std::unique_ptr<Weapon>>                 weapons;
	std::vector<std::unique_ptr<Armor>>                  armors;
	std::vector<std::unique_ptr<Scroll>>                 scrolls;
	std::vector<std::unique_ptr<Location>>               locations;
	
	std::vector<Location*> easy;
	std::vector<Location*> medium;
	std::vector<Location*> hard;
	std::vector<Location*> boss;
};

SnailsvilleData buildSnailsville();
#endif
