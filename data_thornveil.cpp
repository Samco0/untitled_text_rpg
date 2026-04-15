#include "data_thornveil.h"

ThornveilData buildThornveil(){
	ThornveilData d;
	
	auto spell = [&](auto&&... args) -> Spell* {
		d.spells.push_back(std::make_unique<Spell>(std::forward<decltype(args)>(args)...));
		return d.spells.back().get();
	};
	auto chain = [&](auto&&... args) -> ChainingSpell* {
		d.spells.push_back(std::make_unique<ChainingSpell>(std::forward<decltype(args)>(args)...));
		return static_cast<ChainingSpell*>(d.spells.back().get());
	};
	auto lifesteal = [&](auto&&... args) -> LifeStealSpell* {
		d.spells.push_back(std::make_unique<LifeStealSpell>(std::forward<decltype(args)>(args)...));
		return static_cast<LifeStealSpell*>(d.spells.back().get());
	};
	auto hpfx = [&](auto&&... args) -> HpStatusEffect* {
		d.hpEffects.push_back(std::make_unique<HpStatusEffect>(std::forward<decltype(args)>(args)...));
		return d.hpEffects.back().get();
	};
	auto statusSpell = [&](auto&&... args) -> StatusEffectSpell* {
		d.spells.push_back(std::make_unique<StatusEffectSpell>(std::forward<decltype(args)>(args)...));
		return static_cast<StatusEffectSpell*>(d.spells.back().get());
	};
	auto enemy = [&](auto&&... args) -> Enemy* {
		d.enemies.push_back(std::make_unique<Enemy>(std::forward<decltype(args)>(args)...));
		return d.enemies.back().get();
	};
	auto consumable = [&](auto&&... args) -> Consumable* {
		d.consumables.push_back(std::make_unique<Consumable>(std::forward<decltype(args)>(args)...));
		return d.consumables.back().get();
	};
	auto weapon = [&](auto&&... args) -> Weapon* {
		d.weapons.push_back(std::make_unique<Weapon>(std::forward<decltype(args)>(args)...));
		return d.weapons.back().get();
	};
	auto armor = [&](auto&&... args) -> Armor* {
		d.armors.push_back(std::make_unique<Armor>(std::forward<decltype(args)>(args)...));
		return d.armors.back().get();
	};
	auto scroll = [&](auto&&... args) -> Scroll* {
		d.scrolls.push_back(std::make_unique<Scroll>(std::forward<decltype(args)>(args)...));
		return d.scrolls.back().get();
	};
	auto combatLoc = [&](const std::string& name, Enemy* e) -> CombatLocation* {
		d.locations.push_back(std::make_unique<CombatLocation>(name, e));
		return static_cast<CombatLocation*>(d.locations.back().get());
	};
	auto treasureLoc = [&](const std::string& name, const std::string& chest) -> TreasureLocation* {
		d.locations.push_back(std::make_unique<TreasureLocation>(name, chest));
		return static_cast<TreasureLocation*>(d.locations.back().get());
	};
	auto mCombatLoc = [&](const std::string& name, vector<CombatLocation*> locations) -> MultipleCombatLocation* {
		d.locations.push_back(std::make_unique<MultipleCombatLocation>(name, locations));
		return static_cast<MultipleCombatLocation*>(d.locations.back().get());
	};
	auto gCombatLoc = [&](const std::string& name, vector<Enemy*> enemies) -> GroupCombatLocation* {
		d.locations.push_back(std::make_unique<GroupCombatLocation>(name, enemies));
		return static_cast<GroupCombatLocation*>(d.locations.back().get());
	};
	auto healLoc = [&](const std::string& name, const std::string& healName) -> HealLocation* {
		d.locations.push_back(std::make_unique<HealLocation>(name, healName));
		return static_cast<HealLocation*>(d.locations.back().get());
	};
	
}
