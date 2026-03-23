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
	auto combatLoc = [&](const std::string& name, Enemy* e) -> CombatLocation* {
		d.locations.push_back(std::make_unique<CombatLocation>(name, e));
		return static_cast<CombatLocation*>(d.locations.back().get());
	};
	auto treasureLoc = [&](const std::string& name, const std::string& chest) -> TreasureLocation* {
		d.locations.push_back(std::make_unique<TreasureLocation>(name, chest));
		return static_cast<TreasureLocation*>(d.locations.back().get());
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
		Spell* rustTornBolt = nullptr;
		for (auto& sp : d.spells) { if (sp && sp->getName() == "Rust-Torn Scrap Bolt") { rustTornBolt = sp.get(); break; } }
		auto loc = treasureLoc("The Collapsed Watchpost", "Dented Security Lockbox");
		loc->addReward(weapon("Dented Watchman's Baton", "Club", "A heavy enforcement baton, its surface pitted with rust. It still swings true.", 5.0), 45, 1);
		loc->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10.0), 70, 2);
		if (rustTornBolt) loc->addReward(scroll("", rustTornBolt), 30, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 2 | The Gutted Engineer's Cache
	{
		Spell* forgedIronBlow = nullptr;
		for (auto& sp : d.spells) { if (sp && sp->getName() == "Forged Iron Blow") { forgedIronBlow = sp.get(); break; } }
		auto loc = treasureLoc("The Gutted Engineer's Cache", "Oil-Sealed Toolchest");
		loc->addReward(armor("Grease-Slicked Iron Vest", "A padded vest reinforced with sheets of scrap metal and soaked in machine oil. Flexible enough to still move in.", 6.5, 2), 40, 1);
		loc->addReward(consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20.0), 60, 2);
		if (forgedIronBlow) loc->addReward(scroll("", forgedIronBlow), 25, 1);
		loc->setSoulStoneDropChance(9);
		loc->setXpReward(25);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 3 | The Furnace Relic Pit
	{
		Spell* furnaceBreath = nullptr;
		for (auto& sp : d.spells) { if (sp && sp->getName() == "Furnace Breath") { furnaceBreath = sp.get(); break; } }
		auto loc = treasureLoc("The Furnace Relic Pit", "Heat-Warped Iron Chest");
		loc->addReward(weapon("Slag-Tempered Cleaver", "Sword", "A broad cleaver reforged in furnace slag. The metal has a slight orange tinge from the heat it absorbed.", 6.5), 40, 1);
		loc->addReward(consumable("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5), 55, 2);
		loc->addReward(consumable("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10.0), 45, 1);
		if (furnaceBreath) loc->addReward(scroll("", furnaceBreath), 20, 1);
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
		auto e = enemy("Cindermark", 60, 12, 7, 5, 15, 1.25, spells);
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
		auto e = enemy("Slaghand", 65, 13, 7, 3, 15, 1.4, spells);
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
		auto e = enemy("Embervein", 60, 9, 7, 8, 20, 2, spells);
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
		auto e = enemy("Cleavergraft", 68, 15, 7, 2, 15, 1.1, spells);
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
		auto sp2 = statusSpell("Wailing Horn", "A hollow, unnatural horn-call echoes through the air, compelling the target to face the source of the sound.", 12, 4, 75, fx1);
		auto sp3 = chain("Shards of Ruin", "A storm of jagged, glass-like fragments tears through the air, lashing the target in relentless succession.", 0.5, 6, 14, 34, "Glass");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("The Rebuilt", 67, 9, 7, 8, 20, 1.5, spells);
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
		auto e = enemy("Stirrupbone", 70, 11, 7, 8, 10, 1.5, spells);
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
		auto e = enemy("Wrongfit", 62, 13, 7, 9, 20, 1.25, spells);
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
		auto e = enemy("Patience-of-Ash", 65, 7, 7, 3, 20, 2, spells);
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
		auto e = enemy("Ironscald", 62, 6, 7, 8, 20, 2.5, spells);
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
		auto e = enemy("Dredgepull", 72, 13, 7, 3, 25, 1.15, spells);
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
		auto e = enemy("Spade, the Bloodfuel", 67, 8, 7, 4, 15, 2.2, spells);
		e->addReward(consumable("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(armor("Grinning Ember Mask", "A warped, smiling helm stained with soot and heat — its grin never fades, no matter how long it's worn.", 5, 1), 25, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A raccoon who channels fire not through a device, but through their body — blood replaced with a combustible compound, voluntarily. The pain is religious to them.");
		d.medium.push_back(combatLoc("Rustheap Lower Slopes, Bloodfuel Pyre Grounds", e));
	}
	
	// =============================================
	//  HARD ENEMIES
	// =============================================
	
	// Hard 1 | Gravestring
	{
		auto fx1 = hpfx("Resonance Bleed", 5, 2, -4);
		auto sp1 = statusSpell("Resonance Shred", "The guitar's blades spin at resonant frequency, tearing flesh with sound and steel simultaneously.", 14, 4, 75, fx1, "Sound");
		auto sp2 = spell("Chord Collapse", "Every string pulled at once — the resulting shockwave caves in whatever stands in front of it.", 20, 5, "Sound");
		auto sp3 = chain("Staccato Shred", "Short, brutal bursts of resonance-blade strikes, each note a new wound.", 1.5, 4, 6, 10, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Gravestring", 80, 10, 8, 6, 18, 1.5, spells);
		e->addReward(consumable("Wire-Fed Rat Flesh", "Stringy meat laced with thin copper filaments that still faintly hum with residual charge.", 9.5), 40, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(weapon("Resonance Blade Guitar", "Guitar", "A guitar with vibrating resonance blades along each string. Every chord draws blood.", 9), 15, 1);
		e->addReward(armor("Scrap-Studded Stage Coat", "A long coat riveted with scrap metal along the lapels. Worn through more performances than most survive.", 7, 2), 20, 1);
		e->setSoulStoneDropChance(22);
		e->setDescription("An elite raccoon musician turned killer, guitar fitted with resonance blades. Every chord peels plating, ruptures tissue. The performance continues through the screaming.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Shredded Stage", e));
	}
	
	// Hard 2 | Earthcrack
	{
		auto fx1 = hpfx("Ground Tremor", 4, 2, -5);
		auto sp1 = statusSpell("Tectonic Pulse", "A sustained rhythm drives shockwaves deep into the earth, the vibrations travelling up through bone.", 16, 4, 70, fx1, "Physical");
		auto sp2 = spell("Magnitude Slam", "The drummer brings both sticks down simultaneously with seismic force. The ground does not recover.", 22, 5, "Physical");
		auto sp3 = chain("Seismic Cascade", "A relentless drumroll that sends shockwave after shockwave through the target — each beat harder than the last.", 2, 4, 8, 14, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Earthcrack", 85, 11, 8, 3, 16, 1.5, spells);
		e->addReward(consumable("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15), 40, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(weapon("Earthcrack Drumsticks", "Club", "A pair of percussion rods wrapped in iron and used to beat the earth into submission. Each one weighs more than it should.", 9.5), 15, 1);
		e->addReward(armor("Seismic Plated Boots", "Boots reinforced to withstand the kickback of sustained ground-percussion. The soles are cracked from use.", 7, 5), 20, 1);
		e->setSoulStoneDropChance(24);
		e->setDescription("An elite gecko drummer who beats the earth itself. The rhythm is seismic. Those who hear it too long begin to feel it in their bones — then feel nothing at all.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Percussion Crater", e));
	}
	
	// Hard 3 | Voidscream
	{
		auto fx1 = hpfx("Subsonic Rupture", 4, 2, -6);
		auto sp1 = statusSpell("Hemorrhage Scream", "A concentrated sonic blast aimed at the bloodstream, causing vessels to rupture from the inside.", 15, 4, 70, fx1, "Sound");
		auto sp2 = spell("Wall Shatter", "A single scream at a precise frequency that shatters anything brittle — bone, metal, armor, will.", 18, 3, "Sound");
		auto sp3 = chain("Shatter Chorus", "The voice breaks into layered harmonics, each frequency targeting a different weakness, hitting in relentless succession.", 2, 5, 8, 12, "Sound");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Voidscream", 72, 9, 8, 9, 20, 1.6, spells);
		e->addReward(consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10), 40, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(weapon("Voidscream's Resonance Amplifier", "Amplifier", "A neck-mounted resonance device that amplifies vocal output to weaponized levels. Still humming faintly.", 9), 15, 1);
		e->addReward(armor("Soundproof Plated Hood", "A thick padded hood with internal lead lining. Reduces incoming sonic damage. Smells like fear.", 6, 1), 20, 1);
		e->setSoulStoneDropChance(25);
		e->setDescription("An elite vocalist whose screams have been augmented beyond biological limits. The voice tears walls. Those caught in the wave are found with blood in their ears and no memory of the last few moments.");
		d.hard.push_back(combatLoc("Rustheap Upper Slopes, The Screaming Chamber", e));
	}
	
	// =============================================
	//  BOSS
	// =============================================
	
	// Boss | Scrapmonger, the Colossus of Rustheap
	{
		auto fx1 = hpfx("Crushed Internals", 4, 2, -7);
		auto sp1 = statusSpell("Heap Collapse", "Scrapmonger raises a colossal arm and brings the entire weight of the upper heap crashing down upon the target.", 24, 5, 70, fx1, "Physical");
		auto sp2 = spell("Debris Crush", "A mass of compacted refuse is driven forward with the force of a collapsing wall.", 18, 3, "Physical");
		auto sp3 = spell("Tectonic Ram", "Scrapmonger lowers its body and charges — not fast, but with the mass of a collapsed structure behind every step.", 15, 2, "Physical");
		auto sp4 = chain("Shrapnel Cyclone", "The colossus spins, sending a storm of metal fragments outward in an unending lethal spiral.", 1.5, 4, 12, 18, "Metal");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Scrapmonger, the Colossus of Rustheap", 250, 12, 9, 4, 15, 1.5, spells);
		e->addReward(armor("Scrapmonger's Fused Hull Plate", "A chest-sized slab of fused metal pried from Scrapmonger's outer shell. Still warm. Still humming with something.", 14, 2), 60, 1);
		e->addReward(armor("Scrapmonger's Compressed Gauntlets", "Gauntlets formed from condensed refuse, hardened under Scrapmonger's own weight over decades.", 7, 3), 60, 1);
		e->addReward(weapon("Scrapmonger's Debris Arm", "Club", "A massive club-limb broken from the colossus's body. It weighs as much as a person. It swings like it knows.", 12), 40, 1);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp4), 25, 1);
		e->addReward(consumable("Heap-Core Fragment", "A dense, pulsing chunk of compressed refuse from deep within Scrapmonger's body. Consuming it feels like swallowing the heap's history.", 30), 60, 3);
		e->setSoulStoneDropChance(50);
		e->setDescription("No one knows who built Scrapmonger, or if it was built at all — some believe the heap simply accumulated enough refuse to begin thinking. It does not speak. It does not negotiate. It has no goal beyond reducing everything within reach to the same flattened ruin it already inhabits.");
		d.boss.push_back(combatLoc("The Heart of the Heap, Scrapmonger's Domain", e));
	}
	
	return d;
}
