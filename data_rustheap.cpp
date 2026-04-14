#include "data_rustheap.h"

RustheapData buildRustheap() {
	RustheapData d;
	
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
	auto armorfx = [&](auto&&... args) -> ArmorBreakStatusEffect* {
		d.armorEffects.push_back(std::make_unique<ArmorBreakStatusEffect>(std::forward<decltype(args)>(args)...));
		return d.armorEffects.back().get();
	};
	auto tauntfx = [&](auto&&... args) -> TauntStatusEffect* {
		d.tauntEffects.push_back(std::make_unique<TauntStatusEffect>(std::forward<decltype(args)>(args)...));
		return d.tauntEffects.back().get();
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
	auto attackgadget = [&](auto&&... args) -> AttackGadget* {
		d.attackgadgets.push_back(std::make_unique<AttackGadget>(std::forward<decltype(args)>(args)...));
		return d.attackgadgets.back().get();
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
	
	// =============================================
	//  EASY ENEMIES
	// =============================================
	
	// Easy 1 | Gritclaw, the Hollow
	{
		auto sp1 = spell("Rust-Torn Scrap Bolt", "The user conjures a jagged fragment of rust-eaten metal and hurls it violently toward the target.", 8, 3, "Metal");
		auto sp2 = spell("Corrosion Talon Lunge", "Twisted scrap talons form around the user's claws as they leap forward in a savage tearing strike.", 7, 2, "Metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Gritclaw, the Hollow", 46, 5.5, 5, 3, 14, 1.4, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 60, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Scrapfang Talons", "Claws", "Crude claws forged from bent and rusted metal shards.", 5), 15, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A malnourished young raccoon, barely surviving on the lower heaps. Throws corroded junk with trembling claws — desperate and feral, not yet broken enough to stop fighting.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Scrap Burrow", e));
	}
	
	// Easy 2 | Skabscuttle
	{
		auto sp1 = spell("Heapfall", "The user drags a mound of rotting garbage and collapses it onto the target.", 9, 4, "Garbage");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Skabscuttle", 58, 6.5, 5, 2, 15, 1.3, spells);
		e->addReward(consumable("Heaproach Leg", "A severed leg from one of the massive cockroaches that infest the lower rustheaps.", 12.5), 55, 3);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(armor("Heaproach Carapace", "Armor carved from the hardened shell of a giant heap cockroach.", 7, 2), 20, 1);
		e->addReward(armor("Heaproach Skull Helm", "A grotesque helmet fashioned from the hollow head of a giant cockroach.", 4, 1), 10, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A bloated, grime-slick cockroach that infests the heap's underbelly. Shoves rotting debris into paths, more pest than predator — but pests outlive everything.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Garbage Chokepoint", e));
	}
	
	// Easy 3 | Dregfist
	{
		auto sp1 = spell("Livewire Knuckle", "The user coils frayed live wires around their fists, crackling with stolen current before striking.", 8, 2, "Electric");
		auto sp2 = chain("Pulverized Brickstorm", "The user crushes a brittle brick into choking dust and flings the fragments in a violent scatter.", 0.8, 3, 5, 10, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Dregfist", 48, 6, 5, 4, 16, 1.3, spells);
		e->addReward(consumable("Heaproach Leg", "A severed leg from one of the massive cockroaches that infest the lower rustheaps.", 12.5), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Crumbling Mason Brick", "Brick", "A cracked brick pried from the collapsing heaps — crude, heavy, and still deadly.", 4.5), 20, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A low-rung rat who swings crumbling bricks with knuckles wrapped in wire. Part of Rustheap's scavenging underclass — more scarred than skilled, but hungrier than most.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Broken Masonry Runoff", e));
	}
	
	// Easy 4 | Ashfinger
	{
		auto fx1 = hpfx("Suffocating Smoke", 4, 3, 2);
		auto sp1 = statusSpell("Smog Eruption", "The user violently exhales a blast of choking black smoke into the target's face.", 8, 3, 70, fx1, "Smoke");
		auto sp2 = spell("Cinder Knuckle", "The user shapes a dense fist of smoldering ash and slams it into the target.", 9, 3, "Ash");
		auto fx2 = hpfx("Scorching Flames", 4, 2, 2.5);
		auto sp3 = statusSpell("Ember Arrow", "A narrow streak of burning embers is launched toward the target, igniting flesh on impact.", 10, 4, 65, fx2, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Ashfinger", 46, 6, 5, 5, 20, 1.45, spells);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(weapon("Cinderforged Blade", "Sword", "A brittle blade formed from compressed ash and soot.", 4.5), 20, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A twitchy, burn-scarred creature addicted to the smell of smoke. Ignites small fires out of compulsion — some say they no longer feel pain from flame. Their fingertips are permanently charred black.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Smoldering Waste Pit", e));
	}
	
	// Easy 5 | Rattlegrip
	{
		auto sp1 = spell("Scrapshot Discharge", "The user crudely shapes a shard of metal into a makeshift projectile and fires it toward the target.", 8, 3, "Metal");
		auto sp2 = spell("Forged Iron Blow", "The user coats their fist in hardened scrap metal before driving it into the target.", 10, 4, "Metal");
		auto sp3 = spell("Crackling Surge", "A burst of unstable electricity erupts from the user's palm, jolting the target violently.", 7, 2, "Electric");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Rattlegrip", 38, 5, 5, 6, 12, 1.35, spells);
		e->addReward(consumable("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10), 55, 2);
		e->addReward(armor("Rustbound Helm", "A crude helmet hammered together from rusted scrap plates.", 4, 1), 20, 1);
		e->addReward(armor("Patchwork Scrap Cuirass", "A chestplate assembled from mismatched pieces of rusted metal.", 6, 2), 20, 1);
		e->addReward(weapon("Rustheap Torque Wrench", "Wrench", "A heavy industrial wrench once used by Rustheap engineers.", 4.5), 15, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A trembling apprentice barely keeping their contraptions from falling apart. The older engineers beat new techniques into them. Every scar on their hands is a lesson.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Abandoned Technician Shack", e));
	}
	
	// Easy 6 | The Visorless
	{
		auto sp1 = spell("Speed Drive", "The user launches forward in a brutal, bone-rattling charge, slamming into the target with reckless mechanical force.", 11, 5, "Physical");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("The Visorless", 40, 5.5, 5, 6, 15, 1.25, spells);
		e->addReward(consumable("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10), 55, 2);
		e->addReward(scroll("", sp1), 30, 1);
		e->addReward(armor("Fractured Rust Visor", "A cracked protective visor ripped from a scavenger driver somewhere in the lower Rustheap trails.", 4.5, 1), 20, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A skeletal one-seat vehicle stitched from wreck parts and spite. The driver's face is never seen beneath the cracked visor — only teeth, when they charge.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Wreckslide Path", e));
	}
	
	// Easy 7 | Hookwire
	{
		auto sp1 = spell("Rustflail Tail", "The creature lashes its wire-wrapped tail through the air, striking the target with a savage metallic crack.", 9, 3, "Physical");
		auto fx1 = armorfx("Shackled Armor", 1, 4);
		auto sp2 = statusSpell("Chain of Despair", "A jagged spectral chain coils around the target, crushing joints in their armor and weakening its protection.", 7, 3, 70, fx1, "Chains");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Hookwire", 50, 6.5, 5, 5, 20, 1.2, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 55, 3);
		e->addReward(weapon("Barbed Wire Tail", "Club", "A severed raccoon tail stiffened with rusted wire and hooked scrap.", 5), 25, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A young raccoon engineer still learning the heap's dark craft. The tail is wrapped in wire and jagged scrap — not a weapon by design, but by necessity.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Tangled Wire Ravine", e));
	}
	
	// Easy 8 | Splicemite
	{
		auto fx1 = hpfx("Seeping Rot", 4, 1, -3);
		auto sp1 = statusSpell("Rotbreath Miasma", "A foul cloud of rot spores leaks from the creature's broken teeth, drifting into the lungs of its target.", 8, 3, 80, fx1, "Rot");
		auto fx2 = armorfx("Corroded Plating", 1, 3);
		auto sp2 = statusSpell("Corrosive Spittle", "The creature hacks up a string of infected rot and spits it onto the target's armor, slowly eating through the material.", 7, 3, 45, fx2, "Rot");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Splicemite", 34, 4.5, 5, 7, 15, 1.35, spells);
		e->addReward(consumable("Rotroach Leg", "A severed limb from the bloated rustheap cockroaches that crawl through the lower scrap pits.", 12.5), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Infected Spliceblade", "Knife", "A short blade salvaged from a failed augment surgery, still weeping greenish fluid.", 4), 15, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A cockroach whose flesh has been crudely fused with salvaged machine parts — not by choice. The augments are infected, weeping fluid. It bites to spread the rot.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Rotting Augment Pit", e));
	}
	
	// Easy 9 | Rustvow
	{
		auto sp1 = chain("Shattering Glassstorm", "The user summons a storm of jagged glass splinters and flings them toward the target in rapid succession.", 0.8, 3, 7, 12, "Glass");
		auto sp2 = chain("Rusted Bolt Volley", "A cluster of crude metal bolts materializes and is hurled violently toward the target.", 2.5, 4, 3, 5, "Metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Rustvow", 32, 4, 5, 8, 18, 1.5, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A true believer in Rustheap's creed — society is a cage and order is a disease. Throws anything within reach: bolts, teeth, bones, glass.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Shardfall Gutter", e));
	}
	
	// Easy 10 | Gutborer
	{
		auto sp1 = spell("Ground and Pound", "The creature burrows beneath the scrap-choked earth before erupting upward with a brutal uppercut.", 13, 5, "Physical");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Gutborer", 55, 7, 5, 3, 14, 1.45, spells);
		e->addReward(consumable("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5), 55, 2);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(armor("Scrap-Plated Gut Guard", "A curved plate of layered metal covering the midsection, salvaged from wrecked machinery.", 5.5, 2), 20, 1);
		e->setSoulStoneDropChance(7);
		e->setDescription("A worm-shaped construct of unknown origin, writhing beneath the heap's surface. It erupts from below without warning, dragging things into the dark.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Burrowed Scrapbed", e));
	}
	
	// Easy 11 | Shardback
	{
		auto sp1 = spell("Spinebreaker Slam", "The creature twists its shard-covered back and crashes into the target with crushing metallic force.", 11, 3, "Metal");
		auto fx1 = armorfx("Shattered Guard", 1, 4);
		auto sp2 = statusSpell("Razor Tail Flail", "Its jagged tail lashes out wildly, tearing into armor and sending fragments flying.", 9, 3, 70, fx1, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Shardback", 36, 5, 5, 7, 12, 1.5, spells);
		e->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Shard-Edged Tailblade", "Sword", "A curved blade filed from a gecko's detached shard-tail. The edge is uneven but punishing.", 5), 15, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A gecko whose back has fused with metal shards over years of living in the heap. The tail swings not by training, but by rage — a body becoming one with the refuse.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Shardscale Nest", e));
	}
	
	// Easy 12 | Dawe, the Twitch-Copper
	{
		auto sp1 = spell("Static Discharge", "The creature drags its copper-threaded body across scrap, building a violent charge before releasing it in a blinding arc of electricity.", 12, 5, "Electricity");
		auto sp2 = chain("Twitching Surge", "Its body convulses uncontrollably as it flickers in and out of sight, striking the target in erratic bursts of crackling energy.", 2, 4, 3, 5, "Electricity");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Dawe, the Twitch-Copper", 34, 5, 5, 6, 12, 1.4, spells);
		e->addReward(consumable("Wire-Fed Rat Flesh", "Stringy meat laced with thin copper filaments that still faintly hum with residual charge.", 9.5), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Livewire Spine", "Livewire Spine", "A spine threaded with copper veins that twitch with leftover current, as if remembering something it shouldn't.", 5.5), 15, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A rat whose nervous system has been threaded with exposed copper wire. Every twitch sends sparks flying. Some believe they no longer sleep — just convulse. It mutters about taking someone far beyond the sky, somewhere unreachable.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Flickercoil Den", e));
	}
	
	// Easy 13 | Hollowbeat
	{
		auto sp1 = spell("Hollow Resonance", "A deep, unnatural hum escapes the creature, vibrating through bone and scrap alike.", 12, 5, "Sound");
		auto fx1 = armorfx("Disrupted Rhythm", 1, 3);
		auto sp2 = statusSpell("Rhythm Distortion", "The creature emits a warped, broken cadence that throws the target's armor out of sync, weakening its structure.", 9, 3, 70, fx1, "Sound");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Hollowbeat", 50, 6, 5, 5, 14, 1.45, spells);
		e->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Resonance Mallet", "Hammer", "A dense hammer whose head hums with a dull vibration. Strikes land strange.", 4.5), 15, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A gecko who found religion in sound. Beats the ground with its tail in hypnotic rhythms that rattle teeth. The thuds are meditative until they aren't.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Resonance Pit", e));
	}
	
	// Easy 14 | Lowmarrow
	{
		auto fx1 = hpfx("Internal Hemorrhage", 4, 3, -3);
		auto sp1 = statusSpell("Concussive Rumble", "A deep, sub-audible vibration erupts from the creature's ribcage, pulsing through the body and rupturing tissue from within.", 12, 5, 70, fx1, "Sound");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Lowmarrow", 52, 6.5, 5, 3, 12, 1.45, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 55, 3);
		e->addReward(armor("Subsonic Ribcage Plate", "A chestplate formed from a vibrating rib structure that emits a constant, barely audible hum.", 6, 2), 20, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A young raccoon whose ribcage vibrates at sub-audible frequencies. When the sound rises, the air around them shimmers. Internal bleeding is common in those who stand too close.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Subsonic Hollow", e));
	}
	
	// Easy 15 | Woundedge
	{
		auto sp1 = spell("Ragged Carve", "The user drags a broken, jagged edge across the target, tearing flesh in an uneven, brutal cut.", 10, 2, "Physical");
		auto fx1 = hpfx("Jagged Bleeding", 1, 4, -2.5);
		auto sp2 = statusSpell("Serrated Rupture", "A vicious slash from the crude blade leaves a wound that refuses to close, continuing to tear with every movement.", 10, 3, 70, fx1, "Metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Woundedge", 48, 7, 5, 7, 16, 1.3, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 55, 3);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(weapon("Serrated Scrap Blade", "Knife", "A crude blade forged from a shattered augment, its edge uneven and cruel.", 5), 20, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A raccoon who shaped their weapon from their own broken augment after it failed mid-surgery. The blade is ugly and jagged — the anger behind it isn't.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Bloodscrap Alley", e));
	}
	
	// Easy 16 | Blastmite
	{
		auto sp1 = spell("Sudden Ignition", "Without warning, the creature erupts into violent flames, releasing a contained explosion while its own body remains intact.", 13, 5, "Explosion");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Blastmite", 30, 5.5, 5, 8, 12, 1.4, spells);
		e->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 55, 3);
		e->addReward(scroll("", sp1, "Tome"), 25, 1);
		e->addReward(weapon("Igniton Rod", "Club", "A short metal rod packed with volatile compounds at its tip. Handle with care.", 4), 20, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A small gecko packed with improvised charges in the folds of its skin. Expendable. Replaceable. The heap produces more of them every season.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Ignition Nest", e));
	}
	
	// Easy 17 | Bilespew
	{
		auto fx1 = hpfx("Clinging Bile Burn", 5, 3, -2);
		auto sp1 = statusSpell("Scalding Bile Spit", "The creature spews a stream of superheated bile that sticks to flesh, burning long after impact.", 10, 4, 75, fx1, "Fire");
		auto sp2 = spell("Furnace Breath", "A violent stream of fire erupts from the creature's maw, engulfing everything in front of it.", 12, 5, "Fire");
		auto fx2 = hpfx("Blazing Laceration", 4, 2, -2.5);
		auto sp3 = statusSpell("Emberclaw Rend", "The creature forms claws of living flame and tears into the target, leaving burning wounds behind.", 11, 4, 75, fx2, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Bilespew", 58, 7, 5, 5, 14, 1.35, spells);
		e->addReward(consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20), 50, 1);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(weapon("Emberforged Blade", "Sword", "A blade formed from condensed flame, flickering as if barely contained.", 5.5), 20, 1);
		e->setSoulStoneDropChance(7);
		e->setDescription("Born near the heap's furnace pits, their saliva runs hot. Not engineered — adapted. They spit superheated bile that clings to flesh.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Furnace Runoff", e));
	}
	
	// Easy 18 | Ashseat
	{
		auto sp1 = spell("Ash Drive", "The user ignites a burst of ash beneath them, propelling forward in a violent lunge that leaves a choking trail behind.", 12, 5, "Ash");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Ashseat", 38, 5.5, 5, 7, 12, 1.2, spells);
		e->addReward(consumable("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10), 55, 2);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(armor("Ash-Dusted Driver Coat", "A long coat stiffened with ash and dried blood. It smells like the inside of an engine.", 5, 2), 20, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A young driver who inherited their vehicle from a dead sibling. Too small for the seat, but too proud to admit it. The vehicle smells of ash and blood.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Ashtrail Descent", e));
	}
	
	// Easy 19 | Jerkspring
	{
		auto sp1 = spell("Springloaded Kick", "The creature compresses spring-like limbs and releases them in a sudden, snapping kick aimed with unnatural precision.", 13, 6, "Metal");
		auto sp2 = chain("Tick-Twitch Rush", "Its body spasms in rapid mechanical bursts, bouncing erratically as it repeatedly lunges into the target.", 2.5, 4, 2, 4);
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Jerkspring", 34, 5, 5, 8, 10, 1.5, spells);
		e->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 55, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->setSoulStoneDropChance(4);
		e->setDescription("A tiny mechanical gecko, possibly not alive in any traditional sense. It moves in jerky, mechanical ticks. No one knows who built it or why. It attacks on reflex.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Springfracture Ridge", e));
	}
	
	// Easy 20 | Ozone Runt
	{
		auto sp1 = spell("Static Surge", "The user releases a sudden burst of built-up static, striking the target with a sharp electric discharge.", 10, 2, "Electricity");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Ozone Runt", 28, 4, 5, 9, 15, 2.0, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 60, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Charge-Coil Rod", "Club", "A short insulated rod with a crackling copper tip. Touch it wrong and it bites back.", 4), 20, 1);
		e->setSoulStoneDropChance(3);
		e->setDescription("A raccoon who brushed against the Tesla-fields too many times as a child. Their fur stands permanently on end, twitching with residual charge. Speech comes in broken fragments, and the air around them carries the sharp scent of ozone.");
		d.easy.push_back(combatLoc("Rustheap Foothills, Ozone Drift", e));
	}
	
	// Easy Treasure 1 | The Collapsed Watchpost
	{
		auto loc = treasureLoc("The Collapsed Watchpost", "Dented Security Lockbox");
		loc->addReward(weapon("Dented Watchman's Baton", "Club", "A heavy enforcement baton, its surface pitted with rust. It still swings true.", 5.0), 45, 1);
		loc->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10.0), 70, 3);
		loc->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 60, 3);
		loc->addReward(scroll("", spell("Clockwise Break", "The user delivers a rotating strike, the motion grinding forward like a turning gear.", 10, 2, "Physical")), 50, 1);
		loc->addReward(scroll("", spell("Counter-Rotation", "The strike twists against its own motion, snapping back with disorienting force.", 12, 3, "Physical")), 40, 1);
		loc->addReward(scroll("", lifesteal("Chrono Devour", "A warped clock manifests above the user, its hands turning backwards as it siphons fragments of the target’s remaining time.", 14, 4, 80, "Metal")), 30, 1);
		loc->addReward(armor("Watchman's Relic Cap", "A worn cap that hums faintly with the ticking of something unseen.", 4.5, 1), 20, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 2 | The Gutted Engineer's Cache
	{
		auto loc = treasureLoc("The Gutted Engineer's Cache", "Oil-Sealed Toolchest");
		loc->addReward(armor("Grease-Slicked Iron Vest", "A padded vest reinforced with sheets of scrap metal and soaked in machine oil. Flexible enough to still move in.", 6.5, 2), 40, 1);
		loc->addReward(armor("Engineer's Husk Helmet", "A dented helmet lined with soot and dried sweat. The inside feels warmer than it should.", 4.5, 1), 20, 1);
		loc->addReward(consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20.0), 60, 3);
		loc->addReward(weapon("Engineer’s Grip-Wrench", "Wrench", "A heavy wrench worn smooth by years of use. It lands with a dull, bone-shaking impact.", 4), 20, 1);
		loc->addReward(weapon("Rivet Hammer", "Hammer", "A blunt industrial hammer, its head dented from striking more than just metal.", 5), 15, 1);
		loc->addReward(scroll("", chain("Nailstorm", "A violent burst of forged nails tears through the air, driven like shrapnel toward the target.", 1, 4, 5, 12, "Metal")), 40, 1);
		loc->addReward(scroll("", spell("Piercing Beam", "A dense spike of metal is formed and launched with precision, punching clean through resistance.", 13, 3, "Metal")), 40, 1);
		loc->setSoulStoneDropChance(9);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 3 | The Furnace Relic Pit
	{
		auto loc = treasureLoc("The Furnace Relic Pit", "Heat-Warped Iron Chest");
		loc->addReward(weapon("Slag-Tempered Cleaver", "Sword", "A broad cleaver reforged in furnace slag. The metal has a slight orange tinge from the heat it absorbed.", 6.5), 40, 1);
		loc->addReward(consumable("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5), 45, 2);
		loc->addReward(consumable("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10.0), 55, 4);
		loc->addReward(armor("Grease-Slicked Iron Vest", "A padded vest reinforced with sheets of scrap metal and soaked in machine oil. Flexible enough to still move in.", 6.5, 2), 40, 1);
		loc->addReward(armor("Engineer's Husk Helmet", "A dented helmet lined with soot and dried sweat. The inside feels warmer than it should.", 4.5, 1), 20, 1);
		loc->setSoulStoneDropChance(11);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 4 | Ashed Stash
	{
		auto loc = treasureLoc("Burnt Housing, Caved-In Back Room", "Ashed Stash");
		loc->addReward(consumable("Ash-Choked Raccoon Flesh", "Raccoon meat buried under collapse, coated in grey dust. Still edible. Barely.", 9), 70, 3);
		loc->addReward(consumable("Soot-Sealed Gecko Meat", "Dense flesh packed in dried ash — preserved by accident, not intention.", 14), 60, 3);
		loc->addReward(scroll("", statusSpell("Ashen Suffocation", "The user forces a dense cloud of superheated ash directly into the target's throat — it fills the lungs before they can close their mouth.", 13, 4, 80, hpfx("Ashen suffoc", 5, 2, -3), "Ash"), "Tome"), 40, 1);
		loc->addReward(scroll("", spell("Choking Smokeblast", "The user exhales a pressurized torrent of black smoke, thick enough to blind and choke whatever stands in its path.", 15, 5, "Smoke"), "Tome"), 40, 1);
		loc->addReward(scroll("", spell("Ashen Lance", "The user draws ash from the air and compresses it into a rigid, dense spear before driving it forward at killing speed.", 13, 3, "Ash"), "Tome"), 40, 1);
		loc->addReward(weapon("Ash-Packed Spear", "Spear", "A metal spear coated in compressed ash — the layer has hardened into something almost ceramic. It doesn't look like much. It hits like it does.", 4), 20, 1);
		loc->addReward(armor("Ashbound Chestplate", "A chestplate caked in thick, hardened ash from the collapse. Someone wore this when the ceiling came down. They didn't walk out.", 6, 2), 20, 1);
		loc->setSoulStoneDropChance(10);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 5 | The Drained Cistern
	{
		auto loc = treasureLoc("Rustheap Foothills, Drained Cistern", "Waterlogged Iron Crate");
		loc->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 65, 3);
		loc->addReward(scroll("", spell("Pressurized Jet", "The user forces a concentrated stream of scalding water through a crack in their palm, driving it into the target like a drill.", 11, 3, "Water")), 40, 1);
		loc->addReward(scroll("", chain("Pipe Burst", "Corroded pipes rupture along the user's arms, spraying high-pressure bursts that tear into whatever they're aimed at.", 1.5, 4, 3, 7, "Water")), 35, 1);
		loc->addReward(weapon("Corroded Pipe Length", "Club", "A section of heavy iron pipe, corroded through but still solid enough to cave something in.", 4.5), 25, 1);
		loc->addReward(armor("Rust-Sealed Shin Guards", "Leg armor salvaged from a flooded maintenance tunnel. The rust has fused the joints — they don't bend much, but they hold.", 5, 4), 20, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 6 | The Scavenger's Drop Point
	{
		auto loc = treasureLoc("Rustheap Foothills, Collapsed Drop Point", "Scavenger's Tied Bundle");
		loc->addReward(consumable("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10), 60, 3);
		loc->addReward(consumable("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10), 50, 2);
		loc->addReward(scroll("", spell("Thrown Scrap", "The user grabs whatever is within reach and hurls it with enough force to do real damage.", 9, 2, "Metal")), 45, 1);
		loc->addReward(scroll("", chain("Salvage Storm", "The user tears loose handfuls of scrap from the ground and hurls them in rapid succession, each piece finding something soft.", 1, 3, 4, 9, "Metal")), 35, 1);
		loc->addReward(weapon("Scavenger's Hook", "Claws", "A long curved hook lashed to a leather grip. Used for pulling things out of tight spaces — or people.", 4), 20, 1);
		loc->addReward(armor("Patchwork Scrap Cuirass", "A chestplate assembled from mismatched pieces of rusted metal.", 6, 2), 20, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 7 | The Overturned Cart
	{
		auto loc = treasureLoc("Rustheap Foothills, Overturned Transit Cart", "Locked Cargo Box");
		loc->addReward(consumable("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5), 50, 2);
		loc->addReward(scroll("", spell("Cart Slam", "The user drives the full weight of a heavy object into the target — no finesse, just mass and momentum.", 13, 4, "Physical")), 40, 1);
		loc->addReward(scroll("", chain("Ricochet Bolt", "The user fires a dense metal bolt that tears through the target and rebounds off whatever it finds behind them.", 2, 3, 2, 5, "Metal")), 30, 1);
		loc->addReward(weapon("Transit Driver's Ironbar", "Club", "A heavy iron rod used to pin cargo in place. Repurposed. Still works.", 5), 25, 1);
		loc->addReward(armor("Cargo Handler's Shoulder Brace", "A thick shoulder pad reinforced with iron plating, designed to take repeated impact. It has.", 6.5, 2), 20, 1);
		loc->addReward(armor("Worn Transit Boots", "Heavy boots with iron-capped toes, worn flat from years on the heap's transit lines.", 4, 5), 15, 1);
		loc->setSoulStoneDropChance(9);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 8 | The Buried Armory Cache
	{
		auto loc = treasureLoc("Rustheap Foothills, Buried Armory Alcove", "Sealed Weapons Crate");
		loc->addReward(weapon("Heap-Forged Shortsword", "Sword", "A short blade hammered from salvaged iron. The edge is uneven but holds.", 5.5), 35, 1);
		loc->addReward(weapon("Scrap-Bound Hatchet", "Sword", "A hatchet with a blade of layered scrap metal, bound tight with wire. The balance is wrong. The damage isn't.", 5), 30, 1);
		loc->addReward(armor("Dented Iron Helm", "A standard-issue heap helmet, dented from the inside out. Whatever wore it before didn't make it.", 5, 1), 35, 1);
		loc->addReward(armor("Rustbound Helm", "A crude helmet hammered together from rusted scrap plates.", 4, 1), 25, 1);
		loc->addReward(scroll("", spell("Iron Press", "The user drives a dense slab of scrap metal into the target with both hands, the impact flat and final.", 12, 3, "Metal")), 35, 1);
		loc->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 55, 2);
		loc->setSoulStoneDropChance(10);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 9 | The Dead Mechanic's Workshop
	{
		auto loc = treasureLoc("Rustheap Foothills, Dead Mechanic's Workshop", "Grease-Black Strongbox");
		loc->addReward(consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20), 45, 1);
		loc->addReward(weapon("Mechanic's Bone Saw", "Sword", "A serrated blade repurposed from a surgical cutting tool. Someone took it out of context. It works better this way.", 5.5), 25, 1);
		loc->addReward(weapon("Crank-Loaded Hammer", "Hammer", "A hammer with a spring-loaded head. The release mechanism is broken — it fires whenever it wants.", 5.5), 20, 1);
		loc->addReward(armor("Grease-Slicked Iron Vest", "A padded vest reinforced with sheets of scrap metal and soaked in machine oil. Flexible enough to still move in.", 6.5, 2), 30, 1);
		loc->addReward(scroll("", spell("Grinding Gear Strike", "The user drives a spinning gear mechanism into the target, the teeth tearing as it makes contact.", 11, 3, "Metal")), 40, 1);
		loc->addReward(scroll("", chain("Bolt Scatter", "The user loads and releases a cluster of hardened bolts in rapid fire, each one punching through whatever it finds.", 1.5, 3, 3, 8, "Metal")), 35, 1);
		loc->setSoulStoneDropChance(10);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 10 | The Execution Post
	{
		auto loc = treasureLoc("Rustheap Foothills, Abandoned Execution Post", "Bloodstained Lockbox");
		loc->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 60, 3);
		loc->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 50, 2);
		loc->addReward(weapon("Executioner's Cleaver", "Sword", "A broad, heavy blade used at this post for years. The edge is notched from use. The weight does the work.", 6.5), 25, 1);
		loc->addReward(armor("Condemned's Chain Coif", "A hood of rusted chainmail taken from someone who didn't need it anymore.", 5, 1), 30, 1);
		loc->addReward(scroll("", spell("Verdict Strike", "A single, deliberate blow delivered with the full weight of intent behind it — no hesitation, no follow-up.", 14, 4, "Physical")), 35, 1);
		loc->addReward(scroll("", chain("Chain Lash", "The user tears a length of chain from the post and drives it into the target in rapid, whipping strikes.", 1.5, 3, 3, 7, "Chains")), 30, 1);
		loc->setSoulStoneDropChance(11);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// =============================================
	//  MEDIUM ENEMIES
	// =============================================
	
	// Medium 1 | Cindermark
	{
		auto sp1 = spell("Detonation Fist", "The user propels themselves forward with a blast, delivering a punch that detonates violently on impact.", 16, 5, "Explosion");
		auto sp2 = spell("Blaststream", "The user slams their palms together, unleashing a continuous surge of explosions that roars forward like a living inferno.", 13, 3, "Explosion");
		auto sp3 = spell("Flashpoint Grip", "The user reaches for the target's face — and ignites a point-blank explosion the moment contact is made.", 12, 2, "Explosion");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Cindermark", 95, 12, 8, 4, 15, 1.25, spells);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(armor("Blastproof Helmet", "A helmet forged from layered blast-resistant plating.", 5, 1), 25, 1);
		e->addReward(armor("Blastproof Vest", "A heavy vest designed to absorb shockwaves and shrapnel from repeated detonations.", 10, 2), 25, 1);
		e->addReward(weapon("Blastknuckle Gauntlets", "Explosive gloves", "Reinforced gauntlets rigged to detonate on impact, each strike carrying controlled destruction.", 7), 15, 1);
		e->setSoulStoneDropChance(10);
		e->setDescription("A veteran of Rustheap's constant internecine wars, their chest scarred with shrapnel burns — most of them self-inflicted. They no longer flinch at the sound of explosions. Collateral damage stopped mattering long ago.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Blastscar Front", e));
	}
	
	// Medium 2 | Slaghand
	{
		auto fx1 = hpfx("Searing Burn", 5, 2, -6);
		auto sp1 = statusSpell("Molten Slag Burst", "The user hurls a chunk of superheated metal, splashing molten fragments that cling and burn.", 18, 5, 65, fx1, "Molten metal");
		auto sp2 = spell("Rustcleaver", "A heavy, practiced slash driven with brutal efficiency.", 12, 2, "Slash");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Slaghand", 100, 13, 8, 2, 15, 1.4, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->setSoulStoneDropChance(13);
		e->setDescription("A raccoon who works the heap's illegal forges, their hands encased in hardened slag and scar tissue. Every throw of molten metal feels practiced — rhythmic, almost ritualistic. The heat never seems to bother them.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Slagflow Works", e));
	}
	
	// Medium 3 | Embervein
	{
		auto fx1 = hpfx("Hatred Flame", 6, 3, -8.5);
		auto sp1 = statusSpell("Ragefire Burst", "The user unleashes a massive fireball, dense with heat and fury, that erupts on impact.", 16, 4, 85, fx1, "Fire");
		auto sp2 = spell("Molten Impact", "A brutal slam infused with heat, scorching the ground and leaving behind a smoldering crater.", 20, 5, "Fire");
		auto sp3 = chain("Scaleburst Barrage", "The user fires a relentless storm of molten, blade-like scales that tear into the target in rapid succession.", 1, 4, 10, 18, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Embervein", 82, 10, 7, 9, 20, 2.0, spells);
		e->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(armor("Gecko Skin Boots", "Boots crafted from heat-hardened gecko hide.", 5, 5), 25, 1);
		e->addReward(armor("Gecko Skin Gloves", "Gloves made from treated gecko skin, resistant to extreme temperatures.", 5, 3), 25, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A gecko whose internal temperature runs dangerously high. Molten channels pulse beneath thinning scales, glowing faintly with every heartbeat. When its tail strikes the ground, the impact leaves scorched craters — as if the heat within is constantly trying to escape.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Emberfracture Basin", e));
	}
	
	// Medium 4 | Cleavergraft
	{
		auto sp1 = spell("Grafted Uppercut", "The user rapidly forges a crude graft over their arm and drives it upward in a devastating, reinforced strike.", 15, 4, "Metal");
		auto sp2 = spell("Molten Pierce", "The user shapes a spear from dripping molten metal and hurls it forward before it fully solidifies.", 12, 2, "Molten metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Cleavergraft", 105, 15, 8, 2, 15, 1.1, spells);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(armor("Oil-Stained Overalls", "Heavy work overalls stiff with grease, ash, and something darker that never washed out.", 7, 4), 25, 1);
		e->addReward(weapon("Rustbound Graft", "Graft", "A jagged mechanical limb torn from Cleavergraft's body, still faintly twitching.", 8), 15, 1);
		e->setSoulStoneDropChance(15);
		e->setDescription("A veteran Rustheap engineer who replaced a lost hand with a crude graft of metal and scrap. The limb grows, reshapes, and breaks apart mid-combat. Everything they build looks unfinished — and everything they build still works.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Graftwork Yard", e));
	}
	
	// Medium 5 | The Rebuilt
	{
		auto sp1 = spell("Ruinous Impact", "The entity surges forward as if guided by unseen force, its form collapsing into a catastrophic impact that erupts in violent destruction.", 16, 5, "Explosion");
		auto fx1 = tauntfx("Dread Call", 3, 1);
		auto sp2 = statusSpell("Wailing Horn", "A hollow, unnatural horn-call echoes through the air, compelling the target to face the source of the sound.", 12, 4, 75, fx1, "Sound");
		auto sp3 = chain("Shards of Ruin", "A storm of jagged, glass-like fragments tears through the air, lashing the target in relentless succession.", 0.5, 6, 14, 34, "Glass");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("The Rebuilt", 115, 9, 9, 4, 20, 1.4, spells);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->setSoulStoneDropChance(16);
		e->setDescription("A thing that should have remained broken. Twisted metal and bone fused together, rebuilt again and again by unseen hands. It remembers destruction — not as pain, but as purpose. Each movement feels like the echo of a long-forgotten catastrophe.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Field of Broken Echoes", e));
	}
	
	// Medium 6 | Stirrupbone
	{
		auto fx1 = tauntfx("Blinded by Soot", 4, 1);
		auto sp1 = statusSpell("Ashen Drift", "The user exhales a choking veil of ash and smoke, obscuring vision and disorienting the target.", 12, 5, 80, fx1, "Smoke");
		auto sp2 = spell("Rusted Wheel Crush", "A heavy, rust-bound wheel is driven into the target with crushing force, as if part of the user's own body.", 10, 3, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Stirrupbone", 90, 11, 8, 9, 10, 1.5, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A raccoon whose lower body has long since been replaced by iron stirrups and a fused frame of wheels. They no longer walk — only drag or ride. When mounted, their movements become unnaturally fluid, as if guided by something older than instinct.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Ashen Trackway", e));
	}
	
	// Medium 7 | Wrongfit
	{
		auto sp1 = spell("Misfired Relic Cannon", "The user forces a malformed cannon from their own grafted mass and unleashes a violent, unstable blast.", 14, 5, "Metal");
		auto sp2 = spell("Nerve Surge Collapse", "The user's mismatched implants surge uncontrollably, releasing erratic bursts of energy that lash outward.", 12, 4, "Electric");
		auto sp3 = chain("Jagged Gear Rain", "A flurry of twisted, half-formed metal shards erupts and launches forward like a storm of broken mechanisms.", 2, 4, 2, 6, "Metal");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Wrongfit", 85, 12, 7, 10, 20, 1.25, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(weapon("Malformed Gearblade", "Sword", "A warped blade of fused gears that hums with unstable tension.", 7.5), 20, 1);
		e->setSoulStoneDropChance(15);
		e->setDescription("A raccoon who carved and forced foreign machinery into their own body without knowledge or restraint. Nothing aligns. Nothing fits. Yet it lives — sustained by pain, rage, and something that refuses to let the body fail.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Misaligned Workshop", e));
	}
	
	// Medium 8 | Patience-of-Ash
	{
		auto fx1 = tauntfx("Fractured plating", 4, 2);
		auto sp1 = statusSpell("Cinder Implosion", "The user pinches the air itself, birthing a trembling core of unstable heat that collapses inward before violently erupting toward the target.", 12, 4, 80, fx1, "Explosion");
		auto sp2 = chain("Ashen Finger Volley", "The user raises a trembling hand, each finger discharging rapid bursts of volatile force like a ritual gone wrong.", 1.5, 4, 3, 9, "Explosion");
		auto sp3 = spell("Detonation March", "Each step cracks the ground with suppressed blasts as the user advances, culminating in a devastating point-blank eruption upon contact.", 16, 5, "Explosion");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Patience-of-Ash", 80, 8, 8, 2, 20, 2.0, spells);
		e->addReward(armor("Blastwarden Helm", "A scorched helmet worn by those who stand too close to controlled annihilation.", 5.5, 1), 25, 1);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->setSoulStoneDropChance(13);
		e->setDescription("A demolition specialist who doesn't rush. Sets charges with the calm of someone who has accepted all possible outcomes. The countdown is just ceremony.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Ashmarked Detonation Grounds", e));
	}
	
	// Medium 9 | Ironscald
	{
		auto sp1 = spell("Scalding Brandblade", "The user tears molten metal from their own plating, shaping a dripping blade that screams as it cuts through the air.", 13, 3, "Molten metal");
		auto sp2 = chain("Molten Dripstorm", "Globs of liquefied metal spit outward in rapid bursts, clinging and burning as they eat into the target.", 1, 3, 5, 13, "Molten metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Ironscald", 95, 8, 10, 3, 20, 2.5, spells);
		e->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 3);
		e->addReward(armor("Sloughing Steel Carapace", "Molten plating that never fully solidifies, constantly shifting and threatening to collapse off the body.", 8, 2), 25, 1);
		e->addReward(armor("Dripsteel Greaves", "Leg armor that seeps liquid heat with every movement, leaving scorched traces behind.", 6, 4), 25, 1);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->setSoulStoneDropChance(16);
		e->setDescription("A gecko whose body has been deliberately plated with poured steel over living scales. The process is agonizing. The result is a creature that shrugs off blades and radiates heat.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Scalded Plating Grounds", e));
	}
	
	// Medium 10 | Dredgepull
	{
		auto sp1 = spell("Ferrum Grasp", "The user seizes the iron within the target's blood, violently dragging their body across the ground as if pulled by an unseen god.", 16, 6, "Magnetic");
		auto sp2 = spell("Polarity Collapse", "Scrap and rust spiral into the user's fist, forming a dense mass before polarity flips on impact, tearing the construct apart in a violent burst.", 14, 4, "Magnetic");
		auto fx1 = armorfx("Stripped Plating", 5, 1);
		auto sp3 = statusSpell("Gravepull Disarm", "A cruel magnetic surge rips armor from the target's body, casting it aside like shed skin.", 12, 4, 90, fx1, "Magnetic");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Dredgepull", 108, 13, 8, 2, 25, 1.15, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->setSoulStoneDropChance(17);
		e->setDescription("A raccoon with magnetic implants welded into their forearms, pulling shrapnel and refuse into orbit like a grotesque extension of themselves. What they pull, they use to crush.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Ironwake Dredge Fields", e));
	}
	
	// Medium 11 | Spade, the Bloodfuel
	{
		auto sp1 = chain("Rupture Chain", "Each motion tears open volatile veins within the body, releasing chained bursts of unstable force that detonate in erratic succession.", 5, 5, 1, 3, "Explosive");
		auto sp2 = spell("Detonation Breath", "The user exhales a pressurized stream of ignition, each pulse erupting into short-lived but violent explosions.", 13, 3, "Explosive");
		auto sp3 = chain("Ashen Gunsign", "With hollow gestures of a forgotten ritual, the user snaps their fingers into existence, firing bursts of searing force as if the air itself obeyed.", 1, 5, 8, 15, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Spade, the Bloodfuel", 88, 9, 7, 6, 15, 2.2, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(armor("Grinning Ember Mask", "A warped, smiling helm stained with soot and heat — its grin never fades, no matter how long it's worn.", 5, 1), 25, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A raccoon who channels fire not through a device, but through their body — blood replaced with a combustible compound, voluntarily. The pain is religious to them.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Bloodfuel Pyre Grounds", e));
	}
	
	// Medium 12 | Wrongwire
	{
		auto sp1 = chain("Flesh-Bolted Spitter", "A rattling gun-mass erupts from the user's forearm — bolted through skin and bone — and vomits a torrent of jagged scrap in convulsing bursts.", 0.5, 4, 10, 28, "Metal");
		auto sp2 = spell("Marrow Spike", "The user tears a spear of metal free from deep within their own graft, leaving a wet cavity, and hurls it with the force of something that has nothing left to lose.", 12, 2, "Metal");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Wrongwire", 88, 10, 7, 9, 20, 2.0, spells);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(weapon("Livewire Flail", "Whip", "A length of live cable ripped from a dead machine, still twitching, still biting.", 6.5), 20, 1);
		e->setSoulStoneDropChance(14);
		e->setDescription("A mechanic who never learned to fight — only to assemble violence. Their constructs are wrong, wired against reason, yet driven by something that should not be alive.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Miswired Assembly Pit", e));
	}
	
	// Medium 13 | Decaypine
	{
		auto sp1 = lifesteal("Bleed the Current", "A violent surge floods the target's nerves, drinking from them as it burns — the user feeds on what it takes.", 14, 4, 80, "Electric");
		auto sp2 = spell("Spinal Lurch", "The corrupted conduits buried in the user's spine seize all at once, throwing the body forward in a grotesque, twitching lunge.", 12, 2, "Electric");
		auto sp3 = spell("Dead Cross Arc", "The user locks both hands into a rigid cross, fingers blackened, and releases a focused bolt of crackling discharge.", 14, 4, "Electric");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Decaypine", 92, 9, 8, 4, 20, 2.5, spells);
		e->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->setSoulStoneDropChance(15);
		e->setDescription("A gecko whose spine has been overtaken by unstable conduits, pulsing with stolen current. Each discharge feeds it — and destroys it in the same breath.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Conduit Rot Basin", e));
	}
	
	// Medium 14 | Fusedunder
	{
		auto sp1 = spell("Full-Merge Charge", "The user accelerates to full mechanical speed and drives their fused body into the target like a battering instrument with no brakes and no intent to stop.", 16, 5, "Physical");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Fusedunder", 112, 11, 8, 3, 35, 1.25, spells);
		e->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(armor("Fused Throttle Gauntlets", "Heavy gloves grafted to a dead machine's controls — the fingers still know which way to go.", 5.5, 3), 25, 1);
		e->setSoulStoneDropChance(13);
		e->setDescription("A vehicle engineer who fused themselves to their machine during a field repair and never undid the fusion. They drive. They live inside it now.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Wreck-Fused Transit Corridor", e));
	}
	
	// Medium 15 | Cavitybeat
	{
		auto fx1 = hpfx("Ringing ears", 8, 2, -5);
		auto sp1 = statusSpell("Skull Resonance", "The user releases a low, deliberate tone that fills the target's skull and refuses to leave — pressure building behind the eyes until the world tilts.", 12, 4, 80, fx1, "Sound");
		auto sp2 = chain("Marrow Percussion", "The user strikes in rhythmic, compounding waves — each impact carrying a frequency that feels older than the body receiving it.", 2, 5, 3, 8, "Sound");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Cavitybeat", 100, 13, 8, 5, 20, 1.1, spells);
		e->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(weapon("Hollowbone Striker", "Club", "A massive rod of surgically extracted bone, still resonant, still humming with something unspent.", 7), 20, 1);
		e->setSoulStoneDropChance(16);
		e->setDescription("A gecko with hollowed-out bones surgically replaced with resonant chambers. The tail beats produce pressure waves that shatter eardrums at close range.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Resonance Hollow", e));
	}
	
	// Medium 16 | Jawspark
	{
		auto sp1 = spell("Live Bite", "The user drives their electrified jaw into the target — not a clean strike, something that lingers and chews.", 14, 4, "Electric");
		auto sp2 = spell("Throat Discharge", "The user opens their jaw wide and fires a focused beam of electric current, the throat cavity acting as a crude but effective barrel.", 16, 5, "Electric");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Jawspark", 87, 11, 8, 9, 15, 1.5, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(armor("Conductor Jaw", "A replacement jaw fitted with live contacts — it doesn't chew so much as it conducts.", 5.5, 1), 25, 1);
		e->addReward(armor("Charge-Bearing Pauldrons", "Shoulder plating threaded with current, warm to the touch and dangerous to grab.", 8.5, 2), 25, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A raccoon with electric implants running through jaw and shoulders, modified to fire as a weapon. Speaks in crackling static. Eats through the sparks.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Spark-Jaw Scrapyard", e));
	}
	
	// Medium 17 | Spinetorch
	{
		auto fx1 = hpfx("Ignited", 5, 2, -7);
		auto sp1 = statusSpell("Branded Round", "The user selects a single spine with deliberate care and fires it — it will find skin, and what it finds, it keeps burning.", 12, 5, 100, fx1, "Fire");
		auto sp2 = chain("Scatter Volley", "The user releases a torrent of ignited spines in every direction, flooding the space between them and the target.", 1.5, 5, 4, 10, "Fire");
		auto sp3 = statusSpell("Blind Fire", "The user empties their body of spines without aim — most miss, some don't, and the ones that hit still carry heat.", 12, 3, 15, fx1, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Spinetorch", 82, 14, 7, 3, 10, 1.25, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->setSoulStoneDropChance(20);
		e->setDescription("A raccoon with pyrogenic spines grafted along the arms — originally designed for structural support. Someone discovered they could be fired like projectiles. The original designer did not survive the demonstration.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Pyrospine Testing Grounds", e));
	}
	
	// Medium 18 | Threadblast
	{
		auto sp1 = spell("Skin Plug", "The user threads wires into the target's body with surgical precision — then detonates them from the inside.", 14, 4, "Explosion");
		auto sp2 = spell("Trip-Wire Slash", "The user casts a web of live wire and cuts through it — the severed ends detonate on contact with anything soft.", 12, 2, "Explosion");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Threadblast", 85, 14, 7, 3, 10, 1.25, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(weapon("Detcord Lash", "Whip", "A length of detonation cord worn as a weapon — it doesn't crack, it erupts.", 5.5), 20, 1);
		e->setSoulStoneDropChance(14);
		e->setDescription("A gecko whose body is wrapped in detonation wire, much of it threading beneath the skin. They choose, with great care, what to blow up. This discernment is what makes them dangerous.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Detonation Wire Den", e));
	}
	
	// Medium Treasure 1 | The Slagflow Overflow Vault
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Slagflow Overflow Vault", "Heat-Sealed Iron Safe");
		loc->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 60, 3);
		loc->addReward(armor("Slag-Fused Chestplate", "A chestplate that was submerged in molten metal and left to cool. The surface is uneven and scorched. Nothing gets through it.", 9, 2), 35, 1);
		loc->addReward(armor("Forge-Burnt Gauntlets", "Gloves coated in hardened slag, the fingers fused slightly together. You adjust.", 6, 3), 30, 1);
		loc->addReward(scroll("", spell("Molten Coat", "The user dips their arm into a stream of molten metal and drives it into the target before it solidifies.", 14, 3, "Molten metal")), 35, 1);
		loc->addReward(scroll("", statusSpell("Slag Splash", "A bucket of superheated slag tips and pours across the target — it clings, it burns, it doesn't stop.", 16, 4, 70, hpfx("Slag Burn", 5, 2, -5), "Molten metal")), 30, 1);
		loc->setSoulStoneDropChance(14);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 2 | The Abandoned Motor Pool
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Abandoned Motor Pool", "Locked Fleet Chest");
		loc->addReward(consumable("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10), 65, 3);
		loc->addReward(armor("Transit Plate Cuirass", "Chestplate cut from vehicle hull plating. Heavy, ungainly, and almost completely indifferent to damage.", 11, 2), 30, 1);
		loc->addReward(armor("Motor Pool Helmet", "A driver's crash helmet reinforced with welded scrap. The visor is cracked but the shell is intact.", 6, 1), 35, 1);
		loc->addReward(armor("Fleet Driver's Knee Guards", "Thick knee plates salvaged from a transit vehicle's seat frame. Crude mounting. Solid coverage.", 5.5, 4), 25, 1);
		loc->addReward(scroll("", spell("Ramming Speed", "The user drops their shoulder and charges with the momentum of something that was never meant to stop.", 15, 4, "Physical")), 35, 1);
		loc->addReward(scroll("", chain("Wreck Scatter", "Loose vehicle parts tear free and fly outward in a violent burst, each fragment finding a different angle.", 1.5, 4, 3, 8, "Metal")), 30, 1);
		loc->addReward(weapon("Fleet Driver's Crankshaft", "Club", "A heavy crankshaft pulled from a dead engine. The weight is wrong in a way that makes it hit harder.", 7), 25, 1);
		loc->setSoulStoneDropChance(14);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 3 | The Dead Sound Studio
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Dead Sound Studio", "Soundproofed Equipment Locker");
		loc->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 55, 3);
		loc->addReward(scroll("", spell("Feedback Spike", "The user cranks the amplifier past its limit — the resulting shriek of feedback drives into the target's skull like a nail.", 14, 3, "Sound")), 40, 1);
		loc->addReward(scroll("", statusSpell("Low Frequency Pulse", "A sustained bass tone floods the room, rattling organs and blurring vision in anyone caught inside it.", 13, 4, 75, hpfx("Resonance Sickness", 5, 2, -4), "Sound")), 35, 1);
		loc->addReward(scroll("", chain("Dead Air Burst", "The studio goes silent — then erupts in a concentrated wave of compressed sound that hits like a wall.", 2, 4, 3, 7, "Sound")), 30, 1);
		loc->addReward(weapon("Studio Monitor Slab", "Club", "A heavy speaker cabinet torn from the wall. The driver cone still rattles when you swing it.", 6.5), 25, 1);
		loc->addReward(armor("Soundproof Lined Coat", "A long coat with dense acoustic padding sewn into the lining. Muffles incoming sound. Muffles everything.", 7, 2), 25, 1);
		loc->setSoulStoneDropChance(15);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 4 | The Magnetic Impound Yard
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Magnetic Impound Yard", "Flux-Locked Steel Cabinet");
		loc->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 55, 3);
		loc->addReward(scroll("", spell("Pull and Crush", "The user draws a mass of loose metal into the target's body, compressing it inward with magnetic force.", 15, 4, "Magnetic")), 35, 1);
		loc->addReward(scroll("", statusSpell("Polarity Inversion", "The user reverses the magnetic field around the target's armor — the plates begin fighting each other, grinding inward.", 13, 5, 80, hpfx("Crushed Plating", 4, 2, -4), "Magnetic")), 30, 1);
		loc->addReward(armor("Magnetized Hull Vest", "A chestplate salvaged from the impound's crane rig, still faintly magnetized. Small metal objects drift toward it.", 9, 2), 30, 1);
		loc->addReward(armor("Iron-Draw Gauntlets", "Gauntlets threaded with magnetic coils. They pull loose metal toward the fist. Useful. Occasionally inconvenient.", 6, 3), 25, 1);
		loc->addReward(weapon("Impound Crane Hook", "Club", "A massive hooked crane attachment, repurposed. The swing arc is unpredictable. The impact is not.", 8), 20, 1);
		loc->setSoulStoneDropChance(15);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 5 | The Collapsed Graft Clinic
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Collapsed Graft Clinic", "Surgical Supply Lockbox");
		loc->addReward(consumable("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20), 50, 2);
		loc->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 55, 3);
		loc->addReward(scroll("", spell("Surgical Strike", "The user drives a sharpened graft tool into a precise point — not strong, just exact. The damage is internal.", 13, 3, "Metal")), 40, 1);
		loc->addReward(scroll("", statusSpell("Nerve Clamp", "A crude clamp bites into the target's exposed nerve cluster, sending erratic signals through the body that won't stop.", 11, 4, 80, hpfx("Nerve Damage", 5, 3, -3), "Metal")), 35, 1);
		loc->addReward(armor("Clinic-Grade Bone Brace", "A reinforced brace designed to stabilize broken limbs mid-surgery. Repurposed as armor. The straps still work.", 7, 4), 30, 1);
		loc->addReward(weapon("Bone Saw", "Sword", "A surgical cutting blade repurposed as a weapon. The serration is fine and precise. It doesn't stop mid-cut.", 6.5), 25, 1);
		loc->setSoulStoneDropChance(15);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 6 | The Abandoned Rest Area
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Slagflow Overflow Vault", "Heat-Sealed Iron Safe");
		loc->addReward(consumable("Crunchy Rat Remains", "A rat so dry it crunches like old tin when bitten — still smells faintly of the sewers it crawled through.", 15), 50, 3);
		loc->addReward(consumable("Wrathful Cockroach", "Eyes blazing with hatred, this bug seems insulted by the thought of becoming lunch.", 20), 40, 2);
		loc->addReward(consumable("Grease-Soaked Cola", "A bottle of cola floating in a puddle of unidentifiable grease — the fizz fights for survival.", 15), 50, 3);
		loc->addReward(consumable("DIH Cheese Slab", "A chunk of DIH brand cheese, vaguely moldy at the edges, exuding a scent that commands caution.", 25), 30, 1);
		loc->setSoulStoneDropChance(12);
		loc->setXpReward(35);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 7 | The Scrap Corner
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Scrap Corner", "Scrapworker’s Backpack");
		loc->addReward(consumable("Oil-Caked Sandwich", "A sandwich drenched in black oil — bites leave a slick residue on fingers and conscience alike.", 15), 50, 2);
		loc->addReward(consumable("Grease-Soaked Cola", "A bottle of cola floating in a puddle of unidentifiable grease — the fizz fights for survival.", 15), 50, 3);
		loc->addReward(scroll("Tome", spell("Click, Clack, Pow!", "User conjures a makeshift rifle and shoots the target with erratic, yet effective, bursts of scrap metal.", 14, 4, "Metal")), 40, 1);
		loc->addReward(scroll("", chain("Nuts n' Bolts Barrage", "Tiny fragments of scrap hurl themselves toward the enemy, rattling and whirring in chaotic precision.", 1, 5, 3, 15, "Metal")), 40, 1);
		loc->addReward(weapon("Jagged Metal Bolt", "Club", "A massive, jagged bolt ripped from some ruined machine — swings unpredictably and leaves dents in reality.", 7), 20, 1);
		loc->setSoulStoneDropChance(16);
		loc->setXpReward(42);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 8 | The Cooling Tunnel
	{
		auto loc = treasureLoc("Rustheap Lower Slopes, Cooling Tunnel", "Cracked Coolant Container");
		loc->addReward(consumable("Grease-Soaked Cola", "A bottle of cola floating in a puddle of unidentifiable grease — the fizz fights for survival.", 15), 50, 3);
		loc->addReward(scroll("Tome", spell("Icefang Pierce", "User conjures a razor-sharp icicle and drives it into the target with surgical precision — cold enough to bite through sinew and bone.", 16, 5, "Ice")), 40, 1);
		loc->addReward(scroll("Tome", statusSpell("Icy Embrace", "User sends a freezing kiss that gnaws at the target’s skin, each touch sharp and unforgiving. The user remains untouched.", 13, 4, 82, hpfx("Frostbitten", 5, 2, -5), "Ice")), 40, 1);
		loc->addReward(weapon("Frostfang Blade", "Sword", "A jagged sword of ice, sharp enough to slice through sinew and steel alike, leaving a trail of frost in its wake.", 6), 25, 1);
		loc->setSoulStoneDropChance(14);
		loc->setXpReward(37);
		d.medium.push_back(loc);
	}
	
	// =============================================
	//  HARD ENEMIES
	// =============================================
	// Balance target: HP 140-170, dmg 13-19, spd 1-9, level 9
	// Spells: direct 18-27, DOTs -6 to -10/tick, chains 1.5-3 mult
	// Player arrives with ~60-80 HP, dmg 8-14, decent armor
	
	// Hard 1 | Gravestring
	// Speed 6, goes first. 4 spells — Offguard Riff ArmorBreak (85%) sets up Chord Collapse spike.
	// Resonance Bleed DOT (-5/tick x5 = -25 HP) is the sustained threat.
	{
		auto fx1 = hpfx("Resonance Bleed", 5, 2, -5);
		auto sp1 = statusSpell("Resonance Shred", "The guitar's blades spin at resonant frequency, tearing flesh with sound and steel simultaneously — the wound vibrates long after the note dies.", 16, 4, 75, fx1, "Sound");
		auto sp2 = spell("Chord Collapse", "Every string pulled at once. The shockwave doesn't travel — it arrives everywhere simultaneously, caving in whatever stands in front of it.", 24, 5, "Sound");
		auto fx2 = armorfx("Broken Guard", 5, 2);
		auto sp3 = statusSpell("Offguard Riff", "A riff discharged at a frequency the body doesn't expect — the armor rattles loose before the mind registers the sound.", 17, 5, 85, fx2, "Sound");
		auto sp4 = spell("Piercing Note", "A single sustained note, compressed into a needle of pure resonance and driven into the target at killing frequency. No flourish. Just the note, and what it does.", 19, 3, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Gravestring", 145, 14, 9, 6, 18, 1.5, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(weapon("Resonance Blade Guitar", "Guitar", "A guitar with vibrating resonance blades along each string. Every chord draws blood.", 11), 15, 1);
		e->addReward(armor("Scrap-Studded Stage Coat", "A long coat riveted with dense scrap plating along the lapels and shoulders. Worn through more performances than most survive — and it shows.", 9, 2), 25, 1);
		e->addReward(armor("Soundwall Helm", "A reinforced helmet lined with acoustic dampening material torn from the studio walls. Muffles incoming resonance. Muffles most things.", 8, 1), 20, 1);
		e->setSoulStoneDropChance(22);
		e->setDescription("An elite raccoon musician turned killer, guitar strung with resonance blades that peel plating and rupture tissue with every chord. He doesn't perform for an audience anymore. The screaming is enough.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Shredded Stage", e));
	}
	
	// Hard 2 | Christian, the Earthcrack
	// Speed 4 = ties player. Ground Sermon chain avg 6.5x2.5 = ~16 dmg per use.
	// Seismic Unravel ArmorBreak makes Final Measure (22) devastating when armor is stripped.
	{
		auto fx1 = armorfx("Shaken Plating", 4, 2);
		auto sp1 = statusSpell("Seismic Unravel", "The user drives a sustained percussive blast into the target — the vibration travels through armor and finds the joints, rattling them loose.", 13, 4, 80, fx1, "Sound");
		auto sp2 = chain("Ground Sermon", "A conjured drum erupts from the ground and the user beats it without stopping — each strike sends a compounding shockwave that builds until the air itself is shaking.", 2.5, 5, 4, 9, "Sound");
		auto sp3 = spell("Final Measure", "The user stops holding back. One devastating wave, everything behind it, discharged at once — the kind of sound that ends performances.", 22, 4, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Christian, the Earthcrack", 150, 15, 9, 4, 18, 1.4, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(weapon("Earthcrack Drumsticks", "Club", "A pair of iron-wrapped percussion rods used to beat the earth into submission. Each one weighs more than it should. Each one lands harder than it has any right to.", 10), 20, 1);
		e->addReward(attackgadget("Pressurized Beer Can", 30, 25, "Beer Can", "A bloated, dented can of heap brew that looks moments from giving out. It is. Throw it before it decides on its own."), 30, 1);
		e->setSoulStoneDropChance(23);
		e->setDescription("An elite gecko drummer who beats the earth itself. The rhythm is seismic. Those who hear it too long begin to feel it in their bones — then feel nothing at all.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Earthcracking Stage", e));
	}
	
	// Hard 3 | Voidscream
	// Speed 3, player goes first — but Angelic Snare 100% taunt + Octane Blast 100% DOT (-8/tick x6 = -48 HP) is a death sentence if both land.
	// Mechanical Scream (24, CD5) is the direct finisher.
	{
		auto fx1 = tauntfx("Astonished", 5, 1);
		auto sp1 = statusSpell("Angelic Snare", "The user opens with a melody that sounds almost beautiful — the target's body locks in place before the mind understands what happened.", 16, 4, 100, fx1, "Sound");
		auto fx2 = hpfx("Blasted Ears", 6, 2, -8);
		auto sp2 = statusSpell("Octane Blast", "A high-octane note compressed past its natural limit — the eardrums don't rupture so much as give up.", 17, 4, 100, fx2, "Sound");
		auto sp3 = spell("Mechanical Scream", "The user's voice drops into something that was never biological — a sustained mechanical shriek that strips paint, cracks bone, and empties the mind.", 24, 5, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Voidscream", 152, 13, 9, 3, 12, 1.5, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(attackgadget("Handmade AMP Grenade", 40, 50, "Sound Grenade", "A grenade gutted from a salvaged amplifier, packed with resonance coils and spite. The blast is more sound than fire — which makes it worse."), 30, 1);
		e->addReward(armor("Soundproof Plated Hood", "A thick padded hood with internal lead lining. Reduces incoming sonic damage. Smells like fear.", 9, 1), 20, 1);
		e->setSoulStoneDropChance(24);
		e->setDescription("An elite vocalist whose screams have been augmented beyond biological limits. The voice tears walls. Those caught in the wave are found with blood in their ears and no memory of the last few moments.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Screaming Chamber", e));
	}
	
	// Hard 4 | Rudolph, the Iron Chord (Lamb of God + Pantera tributes — spell names stay)
	// Speed 4. By Demons Be Driven DOT (-8/tick x6 = -48 HP) is the run-ender if it procs.
	// More Time To Kill taunt (80%) locks player then fire DOT follows = brutal combo.
	{
		auto sp1 = spell("Another nail for your cofin", "A jagged iron nail is conjured and fired with brutal force, punching deep through flesh and bone.", 17, 3, "Metal");
		auto sp2 = spell("Foot to the throat", "The user surges forward with violent strength, driving a crushing kick straight into the target's throat.", 17, 3, "Physical");
		auto fx1 = tauntfx("Silencing Resonance", 5, 1);
		auto sp3 = statusSpell("More time to kill", "A suffocating hum drowns the air, choking all spellcasting as if the world itself refuses to respond.", 16, 5, 80, fx1, "Sound");
		auto fx2 = hpfx("Infernal Burn", 6, 2, -8);
		auto sp4 = statusSpell("By demons be driven", "Flame tears through the user's body — eyes ignite, horns of fire erupt, and a roaring infernal wave is unleashed.", 19, 5, 80, fx2, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Rudolph, the Iron Chord", 150, 13, 9, 4, 15, 1.35, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(weapon("Stringreaver", "Guitar Axe", "A grotesque fusion of instrument and execution tool — its strings shriek as they carve, tuned to suffering.", 11), 20, 1);
		e->setSoulStoneDropChance(22);
		e->setDescription("A towering executioner of sound and steel. Rhythm is law, and every strike obeys it. The longer the battle lasts, the louder the world becomes — until nothing remains but noise and ruin.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Shrine of the Iron Chord", e));
	}
	
	// Hard 5 | Seismtail
	// Speed 9 — always goes first. Glass cannon: Seismic Rupture ArmorBreak (80%) + Cataclysmic Stone (22).
	// Lower HP compensates for going first and heavy spell damage.
	{
		auto fx1 = armorfx("Fractured Guard", 5, 2);
		auto sp1 = statusSpell("Seismic Rupture", "The ground convulses violently as buried force erupts upward, splintering armor and sending shockwaves through flesh.", 22, 5, 80, fx1, "Ground");
		auto sp2 = spell("Cataclysmic Stone", "A massive slab of earth is torn free and hurled with crushing force, shattering on impact like a falling monument.", 22, 5, "Ground");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Seismtail", 142, 18, 9, 9, 15, 1.25, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(armor("Tremorbound Gauntlets", "Stone-forged gauntlets that pulse with dormant seismic force, each movement carrying the weight of something buried deep.", 7, 3), 20, 1);
		e->setSoulStoneDropChance(26);
		e->setDescription("A hulking gecko whose tail has been reforged into a seismic engine — each strike is not a blow, but a signal. The ground answers. What begins as a tremor ends as collapse. Many have mistaken the first quake for the last warning.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Tremor Scar", e));
	}
	
	// Hard 6 | Bleedfreq
	// Speed 2, player goes first — compensated by Vein Unraveling DOT (-9/tick x5 = -45 HP).
	// Needle Tone (24) and Below-Hearing Dirge (20) keep direct pressure up between DOT ticks.
	{
		auto fx1 = hpfx("Vein Unraveling", 5, 2, -9);
		auto sp1 = statusSpell("Hemorrhage Chorus", "A thin tone settles into the body — then multiplies. Veins loosen their hold on form, coming apart from within.", 19, 5, 80, fx1, "Sound");
		auto sp2 = spell("Needle Tone", "A compressed spike of sound pierces forward, bypassing air and striking somewhere deeper than hearing.", 24, 5, "Sound");
		auto sp3 = spell("Below-Hearing Dirge", "The ground hums. Bones answer. By the time the sound is noticed, something vital has already shifted out of place.", 20, 3, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Bleedfreq", 150, 18, 9, 2, 10, 1.35, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(attackgadget("Resonance Charge", 35, 40, "Sound Grenade", "A hollowed speaker packed with unstable coils. It doesn't explode outward — it collapses everything inward."), 25, 1);
		e->setSoulStoneDropChance(23);
		e->setDescription("Sound made flesh. It does not play music — it disassembles what listens. Victims rarely realize they are dying until movement pulls them apart.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Resonance Vault", e));
	}
	
	// Hard 7 | Crashhymn
	// Speed 3, player goes first. Dissonant Verdict ArmorBreak (80%) + Cathedral Collapse (27) is the combo.
	// Impact Chime (24, CD3) keeps up constant pressure between the big spells.
	{
		auto sp1 = spell("Cathedral Collapse", "A layered wall of sound advances — not as a wave, but as a structure giving way all at once.", 27, 5, "Sound");
		auto fx1 = armorfx("Resonant Fracture", 5, 2);
		auto sp2 = statusSpell("Dissonant Verdict", "A wrong note forced into existence. It finds weakness and widens it.", 23, 5, 80, fx1, "Sound");
		auto sp3 = spell("Impact Chime", "A single metallic strike echoes, repeating faster than it should — each echo landing as force.", 24, 3, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Crashhymn", 155, 16, 9, 3, 15, 1.5, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(armor("Impact-Bound Leggings", "Leg armor built for collision. The material remembers every impact it has survived.", 8, 4), 20, 1);
		e->addReward(armor("Fracture Helm", "A reinforced helm, its surface webbed with cracks that never spread further.", 8.5, 1), 20, 1);
		e->addReward(attackgadget("Overload Amp", 30, 30, "Sound Grenade", "An amplifier forced past its limits. It doesn't burst — it screams."), 25, 1);
		e->setSoulStoneDropChance(25);
		e->setDescription("Where others play sound, this one weaponizes impact itself. Every movement lands twice — once in body, once in structure.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Impact Circuit", e));
	}
	
	// Hard 8 | Slagarchitect
	// Speed 5. Foundry Spill chain avg 10x2 = ~20 dmg. Adhesive Slag DOT (-4/tick x6) + Molten Lance (25) = attrition fight.
	{
		auto sp1 = spell("Molten Lance", "A narrow stream of pressurized molten metal cuts forward with deliberate precision.", 25, 4, "Molten metal");
		auto sp2 = chain("Foundry Spill", "Liquid metal descends in waves, uncontrolled yet somehow guided toward anything that still stands.", 2, 5, 8, 12, "Molten metal");
		auto fx1 = hpfx("Clinging Burn", 6, 3, -4);
		auto sp3 = statusSpell("Adhesive Slag", "Molten residue coats the target and refuses to cool, eating through layer after layer.", 22, 4, 80, fx1, "Molten metal");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Slagarchitect", 152, 14, 9, 5, 15, 1.7, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(weapon("Molt-Blade", "Sword", "A blade that never fully cooled. It shifts subtly, as if still deciding its shape.", 10), 20, 1);
		e->setSoulStoneDropChance(26);
		e->setDescription("Builds with heat, destroys with the same hand. Structures formed by this one are temporary — everything returns to liquid eventually.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Foundry Vein", e));
	}
	
	// Hard 9 | Selfmade Pyre
	// Speed 1 — always goes last. Compensated by two fight-ending DOTs:
	// Sustained Burning (-9/tick x5 = -45 HP) + Boiling Core (-7/tick x6 = -42 HP) stacking = 87 HP DOT total.
	{
		auto fx1 = hpfx("Sustained Burning", 5, 1, -9);
		auto sp1 = statusSpell("Unending Flame", "Both emitters open fully. The fire is not released — it is applied, held in place until resistance fails.", 24, 5, 90, fx1, "Fire");
		auto fx2 = hpfx("Boiling Core", 6, 2, -7);
		auto sp2 = statusSpell("Inner Boil", "Heat is driven inward, bypassing surface and turning what lies beneath against itself.", 22, 4, 80, fx2, "Fire");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Selfmade Pyre", 155, 17, 9, 1, 15, 1.3, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(weapon("Livewire Whip", "Whip", "A heavy cable weapon, its ends frayed and fused. It hums faintly with leftover heat.", 10.5), 20, 1);
		e->addReward(attackgadget("Fuel Flask", 50, 70, "Bottle of Gasoline", "A volatile container of refined fuel. It promises damage. It does not promise control."), 25, 1);
		e->setSoulStoneDropChance(28);
		e->setDescription("They removed their own hands and replaced them with flame. The process failed. The result did not. There is no separation now between wielder and weapon.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Burn Corridor", e));
	}
	
	// Hard 10 | Heartdraw
	// Speed 2. Cardiac Overload (28, CD6) is the hardest single hit in the hard tier.
	// Sustained Conduction chain avg 11.5x1.5 = ~17 dmg. Neural Collapse taunt = no escape.
	{
		auto fx1 = tauntfx("Severed Synapse", 6, 2);
		auto sp1 = statusSpell("Neural Collapse", "A focused arc of living current pierces the skull, scrambling thought into static — the mind refuses to answer its own commands.", 22, 2, 80, fx1, "Electric");
		auto sp2 = spell("Volt-Drawn Arrow", "A bow of crackling current forms mid-air — the released shot tears forward as a screaming line of condensed voltage.", 26, 5, "Electric");
		auto sp3 = chain("Sustained Conduction", "The current latches on and refuses to let go — pulsing, feeding, dragging life through every convulsion.", 1.5, 5, 8, 15, "Electric");
		auto sp4 = spell("Cardiac Overload", "The user channels everything through their failing heart — the discharge erupts outward in a catastrophic beam of pure electrical ruin.", 28, 6, "Electric");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Heartdraw", 162, 18, 9, 2, 15, 1.3, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(attackgadget("Heartcoil Charge", 50, 50, "Electric Relic", "A pulsating coil torn from a failed heart-engine. It discharges violently — the rhythm is unstable, like something trying to keep beating."), 25, 1);
		e->addReward(armor("Nonconductive Carapace", "A chestplate forged from deadened alloys that refuse current — scorched, blackened, and unnaturally silent to the touch.", 10, 2), 20, 1);
		e->addReward(armor("Grave-Touched Insulation Gloves", "Worn by those who worked too close to living current. The fabric hums faintly, as if remembering past shocks.", 7.5, 3), 20, 1);
		e->addReward(weapon("Autopsy Pliers", "Pliers", "A tool once used to extract components from still-living bodies. It grips tighter the more the target resists.", 8), 20, 1);
		e->setSoulStoneDropChance(30);
		e->setDescription("A conductor of their own failing body. Power is drawn not from machines, but from a heart forced beyond design. Each discharge is theft — from themselves. The rhythm falters, but never stops.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Chamber of Failing Pulse", e));
	}
	
	// Hard 11 | Collapselaw
	// Speed 4. Junkfall Construct (28, CD6) is the big hit. Forced Disassembly ArmorBreak (90%) is almost guaranteed.
	{
		auto sp1 = spell("Polarity Crush", "Loose metal tears itself free from the surroundings, slamming together into a dense magnetic fist that crashes into the target with overwhelming force.", 25, 5, "Magnetic");
		auto fx1 = armorfx("Magnetic Strip", 4, 1);
		auto sp2 = statusSpell("Forced Disassembly", "An invisible pull twists violently across the target's body — armor plates rip free and are dragged away, orbiting uselessly in the air.", 22, 4, 90, fx1, "Magnetic");
		auto sp3 = spell("Junkfall Construct", "Scrap surges together mid-air, forming a crude vehicle-shaped mass that is immediately launched forward — not driven, just dropped with intent to crush.", 28, 6, "Metal");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Collapselaw", 160, 15, 9, 4, 15, 1.5, spells);
		e->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->setSoulStoneDropChance(22);
		e->setDescription("A walking center of gravitational collapse — not true gravity, but something close enough to break bodies the same way. Metal bends toward them, obeys them, and dies for them. They have never needed to lift a weapon. The world does it for them.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Collapse Field", e));
	}
	
	// Hard 12 | The Unnamed Weight
	// Speed 3. Crushing Maw (28, CD6) is a massive spike. Smoke Veil taunt + Electrical Disruption ArmorBreak = control then burst.
	{
		auto sp1 = spell("Crushing Maw", "A mass of jagged metal slams together into a brutal, crushing bite — less a jaw, more a collapsing machine.", 28, 6, "Metal");
		auto fx1 = tauntfx("Obscured Vision", 4, 2);
		auto sp2 = statusSpell("Smoke Veil", "Thick industrial smoke floods the area, choking sight and forcing the target into blind reaction.", 22, 4, 80, fx1, "Smoke");
		auto fx2 = armorfx("System Override", 4, 2);
		auto sp3 = statusSpell("Electrical Disruption", "A surge of unstable current rips across the target's gear — systems fail, armor loosens, and protection is stripped away.", 22, 4, 80, fx2, "Electric");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("The Unnamed Weight", 168, 19, 9, 3, 13, 1.3, spells);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(attackgadget("Malfunctioning Motor", 50, 85, "Electric Grenade", "A violently unstable motor core. It hums, shakes, and occasionally sparks — throwing it is a decision you commit to."), 25, 1);
		e->addReward(attackgadget("Cracked Car Battery", 30, 30, "Electric Grenade", "A leaking battery, its casing split open. Acid drips. Charge builds. It will release one way or another."), 25, 1);
		e->addReward(weapon("Severed Steering Lever", "Club", "A snapped steering mechanism repurposed as a blunt weapon. Still slick with oil and resistance.", 10), 20, 1);
		e->setSoulStoneDropChance(28);
		e->setDescription("A fused mass of engine, frame, and something that used to be alive. It does not hunt — it advances. Metal folds into shape as it moves, forming temporary structures of destruction. There is no strategy. Only forward pressure.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Compression Route", e));
	}
	
	// Hard 13 | Ruincalculus
	// Speed 5. Explosive Valley chain avg 13x2 = ~26 dmg. Stressed Out (28, CD6) is the nuke.
	// Precision Detonation ArmorBreak (80%) sets up everything else.
	{
		auto fx1 = armorfx("Structural Failure", 5, 2);
		auto sp1 = statusSpell("Precision Detonation", "A controlled blast is placed with surgical accuracy — the force doesn't scatter, it collapses inward, tearing structure and armor apart.", 23, 5, 80, fx1, "Explosion");
		auto sp2 = chain("Explosive Valley", "A rapid sequence of calculated detonations cascades across the target's position — each blast feeding into the next in a controlled chain reaction.", 2, 5, 10, 16, "Explosion");
		auto sp3 = spell("Stressed Out", "Breathing sharpens. Movements tighten. Every variable locks into place — then the full explosive output is unleashed in a single overwhelming discharge.", 28, 6, "Explosion");
		auto sp4 = spell("Calculated Breach", "A smaller, precisely measured explosion strikes a critical point — minimal force, maximum structural damage.", 21, 3, "Explosion");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Ruincalculus", 162, 16, 9, 5, 15, 1.5, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 2);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(attackgadget("Cherry Charge", 30, 30, "Explosion Grenade", "A compact explosive disguised in a harmless shell. Small, precise, and deceptively destructive."), 25, 3);
		e->setSoulStoneDropChance(25);
		e->setDescription("Every explosion is intentional. Every blast is placed. Ruincalculus does not destroy randomly — they dismantle structures piece by piece, until collapse becomes inevitable.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Demolition Grid", e));
	}
	
	// Hard 14 | Moltenhydra
	// Speed 2, player goes first. Molten Geyser chain avg 20x1 = ~20 dmg per use.
	// Molten Exhale DOT (-7/tick x5 = -35 HP) + Pressurized Discharge (25) = sustained + burst combo.
	{
		auto fx1 = hpfx("Molten Infusion", 5, 2, -7);
		auto sp1 = statusSpell("Molten Exhale", "Superheated liquid metal is expelled in a sustained stream, clinging to the target as it burns through layer after layer.", 23, 5, 80, fx1, "Molten metal");
		auto sp2 = spell("Pressurized Discharge", "A dense burst of molten metal is forced forward under extreme pressure, striking with both heat and impact.", 25, 3, "Molten metal");
		auto sp3 = chain("Molten Geyser", "Multiple tails angle upward — then rupture, releasing a cascading rain of molten metal that crashes down in relentless waves.", 1, 5, 15, 25, "Molten metal");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Moltenhydra", 165, 17, 9, 2, 13, 1.35, spells);
		e->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 50, 4);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->setSoulStoneDropChance(28);
		e->setDescription("Multiple tails. None natural. Each one capable of independent discharge, turning the battlefield into a field of falling heat. The original body was not built for this. It endured anyway.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, Overflow Crucible", e));
	}
	
	// =============================================
	//  HARD TREASURES (6)
	// =============================================
	
	// Hard Treasure 1 | The Hidden Panel (from original)
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Hidden Panel", "Six-Pack of Frostburn Brew");
		loc->addReward(consumable("Frostburn Brew", "A frosty, bitter heap beer that bites back with every gulp — not for the faint-hearted, this pivo will freeze your guts and wake the dead.", 30), 100, 6);
		loc->setSoulStoneDropChance(30);
		loc->setXpReward(65);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 2 | The Soundweaver's Reliquary
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Collapsed Studio Vault", "Soundproofed Steel Reliquary");
		loc->addReward(armor("Resonance-Lined Chestplate", "A chestplate with sound-dampening panels riveted inside. The chest cavity goes quiet when you wear it. Not entirely a comfort.", 12, 2), 40, 1);
		loc->addReward(armor("Soundwall Gauntlets", "Heavy gauntlets lined with acoustic buffer material. They absorb the echo. The impact, less so.", 8, 3), 35, 1);
		loc->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 60, 2);
		loc->setSoulStoneDropChance(32);
		loc->setXpReward(70);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 3 | The Engineer's Black Box
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Buried Engineering Post", "Armored Black Box");
		loc->addReward(weapon("Overclock Baton", "Club", "A regulation enforcement baton retrofitted with a current amplifier. Each hit surges on contact.", 12), 35, 1);
		loc->addReward(armor("Insulated Riot Helm", "A reinforced helmet built for close-range electrical incidents. Still scorched from the last one.", 9, 1), 35, 1);
		loc->addReward(attackgadget("Overcharged Cell", 45, 35, "Electric Grenade", "A power cell pushed past capacity. It's either a weapon or a bad day — often both."), 40, 2);
		loc->setSoulStoneDropChance(30);
		loc->setXpReward(70);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 4 | The Pyroclast Cache
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Buried Pyroclast Shelter", "Heat-Fused Iron Cache");
		loc->addReward(weapon("Molten Spike", "Knife", "A blade cast from a single pour of molten heap metal. It still radiates faint heat from the core.", 11), 35, 1);
		loc->addReward(armor("Slag-Forged Leggings", "Leg armor hardened in a molten runoff pool. The metal settled unevenly — the fit is wrong, the protection isn't.", 10, 4), 35, 1);
		loc->addReward(consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 40), 50, 2);
		loc->addReward(attackgadget("Fuel Flask", 50, 70, "Bottle of Gasoline", "A volatile container of refined fuel. It promises damage. It does not promise control."), 30, 1);
		loc->setSoulStoneDropChance(30);
		loc->setXpReward(70);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 5 | The Upper Scrap Armory
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Sealed Scrap Armory", "Welded-Shut Weapons Crate");
		loc->addReward(weapon("Collapsed Mass Club", "Club", "A dense chunk of fused scrap metal, roughly shaped into something swingable. It doesn't need to be elegant.", 13), 30, 1);
		loc->addReward(armor("Ironwall Cuirass", "A chestplate assembled from interlocked scrap slabs. Ugly by design. The weight alone stops things.", 13, 2), 30, 1);
		loc->addReward(armor("Ironwall Boots", "Boots reinforced with thick metal plating welded to the soles. They slow you down. They also keep you standing.", 9, 5), 30, 1);
		loc->addReward(consumable("Exquisite Gecko Meat", "Dense, almost uncomfortably rich flesh from a gecko that lived hard and long. Still warm. Still humming faintly.", 40), 55, 2);
		loc->setSoulStoneDropChance(33);
		loc->setXpReward(75);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 6 | The Frequency Grave
	{
		auto loc = treasureLoc("Rustheap Upper Slopes, Frequency Testing Chamber", "Lead-Sealed Signal Vault");
		loc->addReward(weapon("Resonance Spike", "Knife", "A narrow blade tuned to vibrate at a specific frequency on impact. It doesn't just cut — it disrupts.", 11), 30, 1);
		loc->addReward(armor("Dead-Signal Hood", "A hood layered with frequency-dampening material recovered from a collapsed broadcast tower. Silence has weight.", 10, 1), 35, 1);
		loc->addReward(attackgadget("Handmade AMP Grenade", 40, 50, "Sound Grenade", "A grenade gutted from a salvaged amplifier, packed with resonance coils and spite. The blast is more sound than fire — which makes it worse."), 35, 2);
		loc->addReward(consumable("Prime Raccoon Flesh", "A thick, well-fed cut of raccoon meat from the upper slopes. Whatever this one ate, it wasn't garbage.", 35), 55, 2);
		loc->setSoulStoneDropChance(32);
		loc->setXpReward(75);
		d.hard.push_back(loc);
	}
	
	// =============================================
	//  BOSS
	// =============================================
	
	// Boss | Scrapmonger, the Colossus of Rustheap
	// 450 HP, dmg 27, speed 4 = ties player.
	// S 300 HP hráče a 20% DR musí Scrapmonger překonat ~240 effectivního HP — base dmg po DR = ~21.6/kolo.
	//
	// Magnetic Reckoning (90% ArmorBreak, 3 stacky) — stripuje DR co nejdřív, pak každý hit bolí víc.
	// Structural Failure DOT (-12/tick x4 = -48 HP) — překoná pasivní regen, nutí hráče spotřebovat consumables.
	// Autonomous Arm Barrage chain (1.5 mult, 2-3 hity) — ~3-4.5 efektivního dmg per hit, ale jede každé 2 kola.
	// Stormcore Discharge (35, CD4) — bez DR to je 35, se zbytky DR stále ~28+.
	// Crushing Augmentation (30, CD6, 95% DOT) — nejnebezpečnější spell, kombuje s Magnetic Reckoning.
	{
		auto sp1 = spell("Stormcore Discharge", "The colossus splits open along its chest — a blinding surge of raw electricity floods outward, turning the battlefield into a living circuit.", 35, 4, "Electric");
		auto sp2 = chain("Autonomous Arm Barrage", "Both arms tear free from the body and launch forward like guided rockets — one slams into the target, then the other follows before they snap violently back into place.", 1.5, 4, 2, 3, "Physical");
		auto fx1 = armorfx("Magnetic Collapse", 3, 3);
		auto sp3 = statusSpell("Magnetic Reckoning", "The surrounding heap screams as metal is torn free — then everything converges inward, ripping armor away as if it never belonged to the target.", 28, 5, 90, fx1, "Magnetic");
		auto fx2 = hpfx("Structural Failure", 4, 2, -12);
		auto sp4 = statusSpell("Crushing Augmentation", "One limb distorts, overbuilt beyond reason — when it lands, the impact doesn't injure, it restructures.", 30, 6, 95, fx2, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Scrapmonger, the Colossus of Rustheap", 450, 27, 10, 4, 15, 1.5, spells);
		e->addReward(armor("Scrapmonger's Fused Hull Plate", "A chest-sized slab of fused metal pried from Scrapmonger's outer shell. Still warm. Still humming with something.", 14, 2), 60, 1);
		e->addReward(armor("Scrapmonger's Compressed Gauntlets", "Gauntlets formed from condensed refuse, hardened under Scrapmonger's own weight over decades.", 7, 3), 60, 1);
		e->addReward(weapon("Collapsed Mass Arm", "Club", "A torn-off limb of the colossus. It resists movement slightly — like it remembers being part of something larger.", 12), 40, 1);
		e->addReward(consumable("Heap-Core Fragment", "A dense, pulsing chunk of compressed refuse from deep within Scrapmonger's body. Consuming it feels like swallowing the heap's history.", 30), 60, 3);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(attackgadget("Scrapmonger's Heart-Core", 60, 45, "Electric Relic", "The exposed heart of the colossus — a violently pulsing core that once drove an entire mass of living scrap. It still beats. It shouldn't."), 25, 1);
		e->setSoulStoneDropChance(50);
		e->setDescription("No one knows who built Scrapmonger, or if it was built at all — some believe the heap simply accumulated enough refuse to begin thinking. It does not speak. It does not negotiate. It has no goal beyond reducing everything within reach to the same flattened ruin it already inhabits.");
		d.boss.push_back(combatLoc("Crown of the Rustheap, Collapse Nexus", e));
	}
	
	
	// Boss | Kaya
	{
		vector<CombatLocation*> combatlocations;
		
		// 1st phase | Kaya, the Soundweaver
		auto sp1 = lifesteal("Pulse Hijack", "Kaya locks onto the target’s heartbeat and drags it into her tempo — every stolen beat feeds her.", 28, 4, 80, "Sound");
		auto sp2 = lifesteal("Resonance Parasite", "A sustained tone embeds itself into the body, draining vitality in perfect harmonic sync.", 26, 3, 80, "Sound");
		auto fx1 = tauntfx("Offbeat", 5, 1);
		auto sp3 = statusSpell("Tempo Break", "Kaya snaps the rhythm unexpectedly — the body hesitates, unable to act outside her timing.", 24, 4, 80, fx1, "Sound");
		auto sp4 = spell("Wall of Noise", "Sound doesn’t travel — it manifests. A crushing wall of raw frequency slams forward instantly.", 30, 5, "Sound");
		Spell* spells1[4] = {sp1, sp2, sp3, sp4};
		auto e1 = enemy("Kaya, the Soundweaver", 160, 18, 10, 3, 20, 1.2, spells1);
		e1->addReward(consumable("Backstage Cut", "Meat taken from something that lived near the stage too long. It pulses faintly when held.", 50), 55, 2);
		e1->addReward(scroll("", sp1), 20, 1);
		e1->addReward(scroll("", sp2), 20, 1);
		e1->addReward(scroll("", sp3), 20, 1);
		e1->addReward(scroll("", sp4), 20, 1);
		e1->addReward(attackgadget("Kaya's Stagebreaker Charge", 40, 35, "Sound Grenade", "A compact sonic core used during her performances — it builds pressure before violently releasing a beat that never should’ve existed."), 35, 2);
		e1->setSoulStoneDropChance(22);
		e1->setDescription("Kaya does not fight — Kaya performs. A master of sonic manipulation who bends the battlefield to her rhythm, trapping foes in polyrhythmic walls of sound, deploying drones that pulse and shriek. The music is the message: this is her stage. You are simply here to suffer in time with the beat.");
		combatlocations.push_back(combatLoc("Crown of the Rustheap, Club of Scorche Frequencies", e1));
		
		// 2nd phase | Kaya, the Pyrosonist
		auto sp5 = lifesteal("Heartbeat Override", "Kaya forces full synchronization — every beat now belongs to her, and every beat drains you.", 30, 5, 100, "Sound");
		auto fx2 = hpfx("Flash Ignite", 5, 1, -8);
		auto sp6 = statusSpell("Snap Ignition", "A simple gesture — the air ignites instantly, as if the beat itself sparked combustion.", 26, 5, 80, fx2, "Fire");
		auto fx3 = hpfx("Cinder Decay", 6, 2, -5);
		auto sp7 = statusSpell("Ashfall Rhythm", "Burning fragments fall in tempo, sticking and consuming everything they touch.", 22, 3, 80, fx3, "Fire");
		auto sp8 = lifesteal("Burning Cadence", "A violent rhythm pulses through the battlefield, draining life with every surge of heat.", 28, 4, 75, "Sound");
		Spell* spells2[4] = {sp5, sp6, sp7, sp8};
		auto e2 = enemy("Kaya, the Pyrosonist", 180, 20, 10, 6, 10, 1.3, spells2);
		e2->addReward(consumable("Charred Encore", "Overcooked, overexposed, yet somehow stronger. The heat never left it.", 55), 55, 2);
		e2->addReward(scroll("", sp5), 20, 1);
		e2->addReward(scroll("", sp6), 20, 1);
		e2->addReward(scroll("", sp7), 20, 1);
		e2->addReward(scroll("", sp8), 20, 1);
		e2->addReward(weapon("Kaya's Encore Blade", "Sword", "Once part of her instrument, now reforged into a weapon. Each strike releases a dying note that lingers just long enough to hurt.", 11.5), 20, 1);
		e2->addReward(armor("Kaya's Stagebreaker Boots", "Boots worn during her final sets. Every step lands slightly off-time — and the world struggles to keep up.", 11, 5), 20, 1);
		e2->addReward(armor("Kaya's Flarevisor", "A heat-scorched visor tuned for performances that ended in fire. Through it, everything moves in rhythm — even the flames.", 12, 1), 20, 1);
		e2->setSoulStoneDropChance(28);
		e2->setDescription("The music breaks. Kaya breaks with it — or becomes something that was always underneath. Flame now moves to the beat, every measure a new swathe of scorched earth. The drones ignite. The arcs breathe in rhythm. The ground burns in time. Those who survive the first stage often cannot run from the second — their legs gave out somewhere between the first chorus and the fire.");
		combatlocations.push_back(combatLoc("Crown of the Rustheap, Club of Scorched Frequencies", e2));
		
		d.boss.push_back(mCombatLoc("Crown of the Rustheap, Club of Scorched Frequencies", combatlocations));
	}
	
	//Dubinek
	{
		vector<CombatLocation*> combatlocations;
		
		// 1st stage | Dubinek, the Mythical Engineer
		auto sp1 = spell("Impact Calibration", "A measured strike — Dubinek adjusts force mid-swing, the blow landing with engineered precision.", 26, 3, "Physical");
		auto fx1 = tauntfx("Target Locked", 3, 1);
		auto sp2 = statusSpell("Threat Broadcast", "A sharp mechanical callout echoes — systems align, and the target becomes priority number one.", 22, 3, 80, fx1, "Sound");
		auto sp3 = chain("Auto-Hammer Sequence", "A conjured hammer system activates — each swing follows the last with machine-like consistency, faster than thought.", 5, 5, 1, 5, "Metal");
		auto fx2 = armorfx("Structural Compromise", 4, 2);
		auto sp4 = statusSpell("Magnetic Field Collapse", "Dubinek drives a magnetized fist into the ground — the resulting field tears at armor, pulling it apart piece by piece.", 24, 6, 80, fx2, "Magnetic");
		Spell* spells1[4] = {sp1, sp2, sp3, sp4};
		auto e1 = enemy("Dubinek, the Mythical Engineer", 185, 19.5, 11, 2, 12, 1.2, spells1);
		e1->addReward(consumable("Engine-Fed Cut", "Dense meat from a creature that fed near active machinery. Tough, rich, and faintly metallic.", 55), 55, 2);
		e1->addReward(scroll("Blueprint", sp1), 20, 1);
		e1->addReward(scroll("Blueprint", sp2), 20, 1);
		e1->addReward(scroll("Blueprint", sp3), 20, 1);
		e1->addReward(attackgadget("Dubinek's Mini Sound Satchel", 35, 25, "Sound Grenade", "A compact charge engineered for controlled sonic bursts. Small, precise, and louder than it has any right to be."), 35, 1);
		e1->addReward(attackgadget("Dubinek's Mini Electric Satchel", 35, 25, "Electric Grenade", "A tightly packed energy cell designed to overload on impact. Stability was never part of the design."), 35, 1);
		e1->addReward(attackgadget("Dubinek's Mini Demolition Satchel", 35, 25, "Explosive Grenade", "A micro-demolition unit used for structural testing. It was never meant for combat. It works anyway."), 35, 1);
		e1->addReward(weapon("Dubinek's Mechanical Hammer", "Hammer", "Mechanical hammer previously held by dubinek", 11), 20, 1);
		e1->setSoulStoneDropChance(29);
		e1->setDescription("The legendary architect of Rustheap's war-machines, Dubinek commands the heap like a workshop. Phase 1 finds them collected — precise, methodical, deploying scrap barrages and autonomous machines with the quiet confidence of someone who has never needed to rush.");
		combatlocations.push_back(combatLoc("Crown of the Rustheap, The Assembly Spine", e1));
		
		// 2nd stage | Dubinek, the Overclocked Berserker
		auto fx3 = hpfx("System Burn", 5, 1, -10);
		auto sp5 = statusSpell("No IDs", "Targeting systems fail to register Dubinek’s movement. The strike lands before recognition — overwhelming, disorienting, unavoidable.", 30, 7, 80, fx3, "Overload");
		auto fx4 = tauntfx("Forced Response", 5, 3);
		auto sp6 = statusSpell("Overdrive Punch", "Motors surge past safe limits — the punch lands with brutal speed, forcing the target into a reactive state they cannot escape.", 25, 5, 90, fx4, "Overload");
		auto sp7 = spell("Videostar", "Velocity exceeds tracking — Dubinek slips past perception itself, reappearing mid-strike with impossible momentum.", 27.5, 4, "Overload");
		auto fx5 = armorfx("Integrity Collapse", 6, 4);
		auto sp8 = statusSpell("Driving With My Eyes Closed", "Sensors shut out — movement driven purely by force and momentum. Defenses crumble under uncontrolled impact.", 25, 5, 90, fx5, "Overload");
		Spell* spells2[4] = {sp5, sp6, sp7, sp8};
		auto e2 = enemy("Dubinek, the Overclocked Berserker", 200, 21, 11, 5, 15, 1.3, spells2);
		e2->addReward(consumable("Overloaded Cut", "Flesh infused with unstable energy. Still warm, still pulsing irregularly.", 55), 55, 2);
		e2->addReward(scroll("Blueprint", sp5), 20, 1);
		e2->addReward(scroll("Blueprint", sp6), 20, 1);
		e2->addReward(scroll("Blueprint", sp7), 20, 1);
		e2->addReward(weapon("Dubinek's Overclock Blade", "Sword", "A weapon warped by excessive output — it hums violently, as if it might tear itself apart mid-swing.", 11), 20, 1);
		e2->addReward(armor("Dubinek's Overclock Visor", "A targeting visor pushed far beyond safe operation. Visual data flickers, but reaction speed is unmatched.", 11, 1), 20, 1);
		e2->addReward(armor("Dubinek's Reinforced Drive Boots", "Boots built to withstand extreme mechanical stress. The ground cracks slightly under each step.", 11, 5), 20, 1);
		e2->setSoulStoneDropChance(33);
		e2->setDescription("Overclocked beyond failure, yet still standing. Systems scream under impossible load, every limit pushed past breaking — yet nothing shuts down. Movements blur into raw output, faster than control, stronger than structure allows. This isn't combat anymore. It's a system in collapse, forcing itself forward, tearing through anything before it can fall apart.");
		d.boss.push_back(combatLoc("Crown of the Rustheap, The Assembly Spine", e2));
		
		d.boss.push_back(mCombatLoc("Crown of the Rustheap, Club of Scorched Frequencies", combatlocations));
	}
	
	// Kaerith and Honnar
	{
		// 1st boss | Kaerith, the Runebinder
		auto sp1 = spell("Runic Projection", "A carved sigil manifests mid-air before launching forward in a precise arc of controlled arcane force.", 25, 5, "Rune");
		auto fx1 = armorfx("Sigil Fracture", 4, 2);
		auto sp2 = statusSpell("Sigil Collapse", "Runes bind to the target’s structure, destabilizing armor as their energy tears through weak points.", 22, 4, 80, fx1, "Rune");
		auto sp3 = chain("Guided Scrap Volley", "Floating sigils redirect surrounding debris into a controlled barrage, each piece striking with unnatural precision.", 2.5, 5, 5, 10, "Metal");
		Spell* spells1[4] = {sp1, sp2, sp3, nullptr};
		auto e1 = enemy("Kaerith, the Runebinder", 140, 18, 11, 6, 15, 1.5, spells1);
		e1->addReward(consumable("Archive-Fed Cut", "Flesh from a creature that fed near Kaerith’s archive. Dense, oddly refined, and faintly humming.", 50), 55, 2);
		e1->addReward(scroll("Runic Fragment", sp1), 20, 1);
		e1->addReward(scroll("Runic Fragment", sp2), 20, 1);
		e1->addReward(scroll("Runic Fragment", sp3), 20, 1);
		e1->addReward(weapon("Kaerith's Arc-Camera", "Camera", "A device that captures and converts visual data into weaponized projections. Every shot feels... calculated.", 11), 20, 1);
		e1->addReward(armor("Kaerith's Binding Hood", "A hood threaded with faintly glowing sigils. It tightens perception, as if the world itself is being annotated.", 11, 1), 20, 1);
		e1->addReward(armor("Kaerith's Archive Gloves", "Precision gloves used for etching runes mid-air. The fingertips hum with stored patterns.", 7, 3), 20, 1);
		e1->setSoulStoneDropChance(25);
		e1->setDescription("The mind of the duo — a rodent archivist of forbidden heap-schematics, etching blueprints and runic formulae into the air itself. Alone, Kaerith cannot withstand pressure. But Kaerith was never built to stand alone. Every rune cast is a command for Honnar.");
		
		// 2nd boss | Honnar, the Ironpaw
		auto fx2 = hpfx("Rending Impact", 6, 2, -7.5);
		auto sp4 = statusSpell("Forged Claw", "A reinforced metal claw forms over Honnar’s limb before tearing into the target with crushing force.", 30, 7, 90, fx2, "Metal");
		auto fx3 = tauntfx("Neural Spike", 5, 1);
		auto sp5 = statusSpell("Shock Directive", "A burst of electricity floods the target’s nervous system, forcing erratic reactions under intense strain.", 22, 5, 90, fx3, "Electricity");
		auto sp6 = spell("Blinding Surge", "A flash of concentrated light erupts outward, overwhelming vision and striking with focused energy.", 22, 3, "Light");
		auto sp7 = spell("Demolition Rush", "Honnar lunges forward with unstoppable momentum, detonating force on impact.", 25, 5, "Explosion");
		Spell* spells2[4] = {sp4, sp5, sp6, sp7};
		auto e2 = enemy("Honnar, the Ironpaw", 240, 21.5, 11, 3, 15, 1.3, spells2);
		e2->addReward(consumable("Raccoon Flesh: Ironbound Cut", "Thick, muscle-dense flesh hardened by constant strain. Heavy, filling, and stubborn.", 50), 55, 2);
		e2->addReward(scroll("Runic Fragment", sp4), 20, 1);
		e2->addReward(scroll("Runic Fragment", sp5), 20, 1);
		e2->addReward(scroll("Runic Fragment", sp6), 20, 1);
		e2->addReward(scroll("Runic Fragment", sp7), 20, 1);
		e2->addReward(armor("Honnar's Reinforced Chestplate", "A heavy plate built to endure direct impact. Scratches run deep — none broke through.", 13.5, 2), 20, 1);
		e2->addReward(armor("Honnar's Load-Bearing Leggings", "Leg armor designed for stability under extreme force. Movement feels grounded, unstoppable.", 9, 4), 20, 1);
		e2->addReward(armor("Honnar's Impact Boots", "Each step carries weight — the ground responds accordingly.", 11, 5), 20, 1);
		e2->addReward(attackgadget("Honnar's Breach Charge", 40, 30, "Explosion Grenade", "A crude but effective explosive used to break through reinforced structures."), 25, 3);
		e2->setSoulStoneDropChance(35);
		e2->setDescription("The body of the duo — Kaerith's designs made flesh. Honnar does not question. Honnar executes. The hammer falls where the runes point. The claw follows where the blueprints demand. Without Kaerith, Honnar is devastation without direction. With Kaerith, Honnar is architecture.");
		
		vector<Enemy*> group = {e1, e2};
		d.boss.push_back(gCombatLoc("Crown of the Rustheap, The Runic Assembly", group));
	} 
	
	return d;
}
