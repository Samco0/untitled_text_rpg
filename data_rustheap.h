#ifndef DATA_RUSTHEAP_H
#define DATA_RUSTHEAP_H
#pragma once
#include <vector>
#include <memory>
#include "map.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "weapon.h"

struct RustheapData {
	std::vector<std::unique_ptr<Spell>>                  spells;
	std::vector<std::unique_ptr<HpStatusEffect>>         hpEffects;
	std::vector<std::unique_ptr<ArmorBreakStatusEffect>> armorEffects;
	std::vector<std::unique_ptr<TauntStatusEffect>>      tauntEffects;
	std::vector<std::unique_ptr<Enemy>>                  enemies;
	std::vector<std::unique_ptr<Consumable>>             consumables;
	std::vector<std::unique_ptr<Weapon>>                 weapons;
	std::vector<std::unique_ptr<Armor>>                  armors;
	std::vector<std::unique_ptr<Scroll>>                 scrolls;
	std::vector<std::unique_ptr<AttackGadget>>           attackgadgets;
	std::vector<std::unique_ptr<Location>>               locations;
	
	std::vector<Location*> easy;
	std::vector<Location*> medium;
	std::vector<Location*> hard;
	std::vector<Location*> boss;
};

RustheapData buildRustheap();
#endif
