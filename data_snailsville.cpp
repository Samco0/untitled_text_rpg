#include "data_snailsville.h"

// Helper macros to reduce boilerplate
// mkSpell / mkChain / mkLifesteal / mkStatus / mkStatusSpell all push_back into d.spells
// and return a raw pointer for immediate use.

SnailsvilleData buildSnailsville() {
	SnailsvilleData d;
	
	// Convenience lambdas — push into ownership store and return raw ptr
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
	auto healLoc = [&](const std::string& name, const std::string& healName) -> HealLocation* {
		d.locations.push_back(std::make_unique<HealLocation>(name, healName));
		return static_cast<HealLocation*>(d.locations.back().get());
	};
	
	// =============================================
	//  EASY ENEMIES
	// =============================================
	
	// Easy 1 | Sputter the Smith
	{
		auto sp1 = spell("Cracked Hammer", "The user shapes a jagged, darkened hammer from twisted metal and strikes the target's skull with a resonant clang.", 4, 3, "Physical");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Sputter the Smith", 14, 1.5, 1, 3, 10, 1.3, spells);
		e->addReward(consumable("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5), 60, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(weapon("Blackened Shell Sword", "Sword", "Forged from shell and blackened iron, it hums faintly with a restless energy.", 2.5), 15, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A hunched snail-smith blackened by years of forge-smoke, its shell cracked and scorched. It swings a hammer with the slow patience of something that has never known hurry.");
		d.easy.push_back(combatLoc("The Snailsmith's Forge", e));
	}
	
	// Easy 2 | Shellcarver Apprentice
	{
		auto sp1 = spell("Shattered Rock", "The user hurls a jagged fragment of stone, cracking it against the target with a sharp impact.", 5, 3, "Physical");
		auto sp2 = chain("Pebble Barrage", "A swarm of sharp, unnatural pebbles erupts from the user's hands, cutting at the target relentlessly.", 0.75, 4, 3, 8, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Shellcarver Apprentice", 15, 1.75, 2, 3, 10, 1.3, spells);
		e->addReward(consumable("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5), 65, 3);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 15, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A young apprentice still learning the cruel craft, its soft shell barely hardened. It flings stones with frantic, twitching hands — not skilled, but relentless.");
		d.easy.push_back(combatLoc("A Pile of Rocks..?", e));
	}
	
	// Easy 3 | Glimmering Courier
	{
		auto sp1 = spell("Blinding Flash", "The user emits a sudden, piercing light that sears the target's eyes, leaving a lingering sting.", 3, 2, "Light");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Glimmering Courier", 9, 1.25, 2, 7, 12, 1.4, spells);
		e->addReward(consumable("Glimmering Snail Flesh", "A chunk of snail flesh, glinting faintly as if it holds a strange, inner light.", 10), 75, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A slight, swift snail whose shell catches the light with an eerie inner glow. It darts with unsettling speed, the light from its body flickering like a candle about to die.");
		d.easy.push_back(combatLoc("The Lighthouse", e));
	}
	
	// Easy 4 | Herbalist Snail
	{
		auto sp1 = spell("Petal Maelstrom", "The user summons a spiraling storm of brittle, blackened petals that slice at anything in its path.", 4, 3, "Nature");
		auto sp2 = spell("Thorn Lash", "The user lashes thorned vines at the target, tearing through flesh and armor alike.", 3, 2, "Nature");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Herbalist Snail", 12, 1.5, 1, 5, 10, 1.25, spells);
		e->addReward(consumable("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5), 50, 3);
		e->addReward(consumable("Withered Rose", "A decayed, blackened rose oozing a faint, sticky residue.", 12.5), 30, 1);
		e->addReward(scroll("", sp1), 15, 1);
		e->addReward(weapon("Thorned Vine Whip", "Whip", "A whip braided from thorned vines, dark and twisted, leaving deep scratches.", 2), 5, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A gaunt snail draped in withered vines and rotting petals. It tends to blackened herbs with a tenderness that makes its violence feel like a betrayal.");
		d.easy.push_back(combatLoc("The Greenhouse", e));
	}
	
	// Easy 5 | Lampbearer Snail
	{
		auto sp1 = spell("Searing Gleam", "The user unleashes a piercing flash of corrupted light, scorching eyes and leaving a lingering daze.", 4, 3, "Fire");
		auto sp2 = chain("Waxstorm", "The user trembles violently, raining molten wax shards that stick and burn, scalding flesh and armor alike.", 0.5, 4, 3, 10, "Fire");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Lampbearer Snail", 14, 1.75, 1, 4, 10, 1.25, spells);
		e->addReward(consumable("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5), 60, 3);
		e->addReward(weapon("Slimebound Lantern", "Lamp", "A warped lantern covered in snail slime; its flickering light dances unnaturally across the walls.", 1.5), 30, 1);
		e->addReward(scroll("", sp2), 10, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A broad snail bearing a warped lantern fused into its shell, leaking pale light and the stench of burnt wax. The flame flickers with its breath, as if sharing one slow, shared life.");
		d.easy.push_back(combatLoc("The Lampbearer's Lair", e));
	}
	
	// Easy 6 | Tavern Worker Snail
	{
		auto sp1 = spell("Barrel Smash", "The snail conjures a warped, oozing barrel and hurls it at full speed, splintering whatever it hits.", 6, 4, "Physical");
		auto sp2 = spell("Piercing Brew", "The snail slaps its hands together, forming a condensed stream of fermented sludge that pierces the target.", 4, 3, "Dark");
		auto sp3 = spell("Shattered Plate", "A jagged plate materializes in the snail's hand and flies like a deadly disc at the target.", 3, 2, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Tavern Worker Snail", 18, 2, 3, 3, 10, 1.25, spells);
		e->addReward(consumable("Muccus Ale", "A thick, murky brew that sloshes with unsettling bubbles; not for the faint-hearted.", 12.5), 35, 2);
		e->addReward(consumable("Slime Wine", "A viscous, dark alcoholic concoction that burns like molten tar.", 15), 25, 2);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 10, 1);
		e->addReward(scroll("", sp3), 10, 1);
		e->setSoulStoneDropChance(7);
		e->setDescription("A thick-set tavern snail reeking of muccus brew and stale violence. The apron it wears is stained in colours that are not wine. It fights with the casual brutality of someone who has done this before closing time.");
		d.easy.push_back(combatLoc("The Rotting Tavern", e));
	}
	
	// Easy 7 | Fisher Snail
	{
		auto sp1 = spell("Mire Bubble", "The snail exhales a swollen, murky bubble of mucus that drifts forward before bursting violently against the target.", 4, 3, "Water");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Fisher Snail", 13, 1.5, 1, 3, 10, 1.25, spells);
		e->addReward(consumable("Mirelake Catch", "A pale fish dragged from the depths of the mucus lake. Its skin feels wrong to the touch.", 7.5), 50, 3);
		e->addReward(scroll("", sp1), 30, 1);
		e->addReward(weapon("Mirebound Fishing Rod", "Fishing Rod", "A warped rod stiffened by dried slime and lake residue. It smells faintly of rot.", 1.75), 20, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A pale, wiry snail crouched at the edge of the mucus lake, fishing rod still clutched in hand. Its eyes are glassy and far away, as if whatever it was waiting to catch already took something from it.");
		d.easy.push_back(combatLoc("The Mirelake Hut", e));
	}
	
	// Easy 8 | Mud Leech
	{
		auto sp1 = lifesteal("Sanguine Leech", "A writhing, vein-thick tendril lashes out and drains vitality straight from the victim's veins.", 6, 4, 60, "Blood");
		auto sp2 = spell("Grave Mud Slam", "A hulking mass of grave-cold mud forms into a hand and crashes down upon the target.", 4, 3, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Mud Leech", 14, 1.75, 2, 4, 10, 1.25, spells);
		e->addReward(consumable("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10), 60, 3);
		e->addReward(scroll("", sp1), 40, 1);
		e->setSoulStoneDropChance(6);
		e->setDescription("A bloated, mud-slicked creature more leech than snail, its body pulsing with stolen warmth. Where it crawls, the earth darkens. Its touch leaves a cold that lingers long after the wound closes.");
		d.easy.push_back(combatLoc("The Drowned Hollow", e));
	}
	
	// Easy 9 | Tiny Rotworm
	{
		auto sp1 = lifesteal("Frail Siphon", "A thin tendril pierces the flesh and steals a whisper of vitality.", 4, 3, 50, "Blood");
		auto sp2 = spell("Blight Spit", "The worm spits a string of foul, decaying bile at the target.", 3, 2, "Dark");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Tiny Rotworm", 8, 1.25, 1, 5, 8, 1.2, spells);
		e->addReward(consumable("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10), 60, 3);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->setSoulStoneDropChance(5);
		e->setDescription("A tiny, wretched thing barely the size of a fist, almost pitiable. Its body is translucent enough to see the dark fluid moving inside. It bites like something with nothing left to lose.");
		d.easy.push_back(combatLoc("The Rotpit Burrow", e));
	}
	
	// Easy Treasure 1 | The Snailsmith's Backroom
	{
		auto sp1 = d.spells[0].get(); // Cracked Hammer — reuse Easy 1's spell
		auto loc = treasureLoc("The Snailsmith's Backroom", "Soot-Stained Lockbox");
		loc->addReward(weapon("Soot-Dusted Tack Hammer", "Hammer", "A battered forge hammer still warm from the coals, left behind by a smith who never came back.", 2.0), 50, 1);
		loc->addReward(consumable("Forge Marrow", "A dense lump of rendered snail tallow pressed into a waxy cake. Tastes of iron. Keeps you moving.", 10.0), 70, 2);
		loc->addReward(scroll("", sp1), 30, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(20);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 2 | The Abandoned Post Route
	{
		auto sp1 = d.spells[4].get(); // Blinding Flash — reuse Easy 3's spell
		auto loc = treasureLoc("The Abandoned Post Route", "Dropped Courier Satchel");
		loc->addReward(armor("Courier's Wax-Sealed Cap", "A light cap lacquered in hardened snail wax, faintly luminescent.", 1.0, 1), 40, 1);
		loc->addReward(consumable("Glimmer Salve", "A pale, faintly glowing ointment smeared from Glimmering Courier flesh. Closes wounds without asking questions.", 12.0), 65, 2);
		loc->addReward(scroll("", sp1), 25, 1);
		loc->setSoulStoneDropChance(8);
		loc->setXpReward(20);
		d.easy.push_back(loc);
	}
	
	// Easy Treasure 3 | The Rotting Tavern Cellar
	{
		auto sp1 = d.spells[12].get(); // Barrel Smash — reuse Easy 6's spell
		auto loc = treasureLoc("The Rotting Tavern Cellar", "Wax-Sealed Barrel Cache");
		loc->addReward(armor("Grease-Stained Plate Apron", "A thick apron hammered flat and riveted with shell scraps. Smells of old brew and worse things.", 1.5, 2), 35, 1);
		loc->addReward(consumable("Muccus Ale", "A thick, murky brew that sloshes with unsettling bubbles; not for the faint-hearted.", 12.5), 60, 2);
		loc->addReward(consumable("Slime Wine", "A viscous, dark alcoholic concoction that burns like molten tar.", 15.0), 40, 1);
		loc->addReward(scroll("", sp1), 20, 1);
		loc->setSoulStoneDropChance(10);
		loc->setXpReward(20);
		d.easy.push_back(loc);
	}
	
	// Easy Heal 1 | Rusted Prayer Frame
	{
		auto loc = healLoc("Rusted Prayer Road", "Rusted Prayer Frame");
		d.easy.push_back(loc);
	}
	
	// Easy Heal 2 | Broken Shell Totem
	{
		auto loc = healLoc("Cracked Shell Path", "Broken Shell Totem");
		d.easy.push_back(loc);
	}
	
	// Easy Heal 3 | Mucus Stone Idol
	{
		auto loc = healLoc("Slimestone Crossing", "Mucus Stone Idol");
		d.easy.push_back(loc);
	}
	
	// Easy Heal 4 | Quiet Lantern Husk
	{
		auto loc = healLoc("Faded Light Trail", "Quiet Lantern Husk");
		d.easy.push_back(loc);
	}
	
	// =============================================
	//  MEDIUM ENEMIES
	// =============================================
	
	// Medium 1 | Ironshell Knight
	{
		auto sp1 = spell("Ferric Impact", "The knight encases his gauntlet in hardened iron and delivers a crushing blow.", 6, 3, "Physical");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Ironshell Knight", 28, 2.5, 3, 3, 10, 1.4, spells);
		e->addReward(armor("Helm of the Ironshell", "A heavy helmet hammered from layered ironshell plating.", 2.5, 1), 20, 1);
		e->addReward(armor("Cuirass of the Ironshell", "A reinforced chestplate forged from the knight's own shell.", 4, 2), 20, 1);
		e->addReward(armor("Ironshell Gauntlets", "Gauntlets of dense metal plating, still scarred from battle.", 1.5, 3), 20, 1);
		e->addReward(armor("Ironshell Greaves", "Leg armor shaped from hardened shell segments.", 3, 4), 20, 1);
		e->addReward(armor("Ironshell Sabatons", "Boots plated in iron, heavy with purpose.", 2, 5), 20, 1);
		e->addReward(scroll("", sp1), 10, 1);
		e->setSoulStoneDropChance(10);
		e->setDescription("A knight sealed inside a shell of layered iron plate, slow as a funeral procession. Its face is hidden entirely behind riveted metal. Whatever it once was has long since rusted over.");
		d.medium.push_back(combatLoc("The Ironbound Hall", e));
	}
	
	// Medium 2 | Shadow Sentry
	{
		auto sp1 = spell("Umbral Kick", "A swift kick wreathed in condensed shadow energy.", 5, 2, "Dark");
		auto sp2 = spell("Gloomblade", "A blade of hardened darkness forms and cleaves through the target.", 7, 3, "Dark");
		auto sp3 = chain("Eclipse Barrage", "Weapons fall aside as the sentry's fists become void-shrouded, striking in relentless succession.", 1, 3, 1, 5, "Dark");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Shadow Sentry", 24, 2.5, 3, 6, 12, 1.4, spells);
		e->addReward(consumable("Condensed Shadow Flesh", "Cold, elastic meat infused with lingering darkness.", 15), 55, 3);
		e->addReward(scroll("", sp1), 15, 1);
		e->addReward(scroll("", sp2), 15, 1);
		e->addReward(scroll("", sp3), 15, 1);
		e->setSoulStoneDropChance(11);
		e->setDescription("A lean sentry wrapped in shadow like a second skin, barely visible until it moves. Its eyes are the only thing that catches light — two pale embers hovering in the dark where a face should be.");
		d.medium.push_back(combatLoc("The Shaded Hollow", e));
	}
	
	// Medium 3 | Leech Scout
	{
		auto sp1 = lifesteal("Crimson Siphon Strike", "A brutal tendril-infused blow that tears vitality straight from the veins.", 7, 4, 70, "Blood");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Leech Scout", 22, 2.5, 3, 5, 12, 1.4, spells);
		e->addReward(consumable("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10), 75, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->setSoulStoneDropChance(10);
		e->setDescription("A lithe, scarred leech that moves with a hunter's economy, leaving faint trails of crimson where it passes. It does not fight out of hunger. It has simply never learned to stop.");
		d.medium.push_back(combatLoc("The Drowned Vein-Tunnel", e));
	}
	
	// Medium 4 | Obsidian Shell Snail
	{
		auto sp1 = spell("Shadowfist Strike", "The user envelops their claw in living shadow and slams it into the foe, chilling the soul", 6, 3, "Dark");
		Spell* spells[4] = {sp1, nullptr, nullptr, nullptr};
		auto e = enemy("Obsidian Shell Snail", 26, 2.5, 3, 4, 10, 1.4, spells);
		e->addReward(armor("Helm of Eternal Night", "A blackened helm that whispers with the voices of the damned", 2.5, 1), 20, 1);
		e->addReward(armor("Cursed Carapace", "An obsidian chestplate etched with runes that sap warmth from the wearer", 4, 2), 20, 1);
		e->addReward(armor("Gauntlets of Umbral Grip", "Shadow-forged gauntlets that chill to the touch", 1.5, 3), 20, 1);
		e->addReward(armor("Leggings of the Hollow Shell", "Dark leggings that seem to absorb light around them", 3, 4), 20, 1);
		e->addReward(armor("Boots of the Blackened Trail", "Boots that leave faint trails of shadow behind with every step", 2, 5), 20, 1);
		e->addReward(scroll("", sp1, "Tome"), 10, 1);
		e->setSoulStoneDropChance(12);
		e->setDescription("A snail encased in obsidian shell so dark it seems to swallow the light around it. Standing near it feels like standing at the edge of something you cannot name. The cold it radiates is not temperature.");
		d.medium.push_back(combatLoc("Obsidianbound Hall of Whispers", e));
	}
	
	// Medium 5 | Venomspire Snail
	{
		auto fx1 = hpfx("Corrosive Venom", 5, 1, -1);
		auto sp1 = statusSpell("Venomous Fang Strike", "A fang coated in a writhing black venom tears into the target, burning flesh and soul alike", 6, 4, 70, fx1, "Dark");
		auto fx2 = hpfx("Blight Poison", 3, 1, -0.75);
		auto sp2 = statusSpell("Blighted Jab", "A quick strike that injects a creeping poison, leaving the target weakened and shivering", 4, 3, 70, fx2, "Dark");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Venomspire Snail", 24, 2, 3, 4, 12, 1.4, spells);
		e->addReward(consumable("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak", 7.5), 50, 3);
		e->addReward(scroll("", sp1, "Tome"), 20, 1);
		e->addReward(scroll("", sp2, "Tome"), 20, 1);
		e->addReward(weapon("Shard of Venomspire Shell", "Knife", "A jagged shard from the snail's shell, still pulsing with dark energy", 2), 10, 1);
		e->setSoulStoneDropChance(10);
		e->setDescription("A tall, needle-shelled snail bristling with hollow spines that weep black venom in steady drops. It moves with quiet deliberation, as if it already knows the outcome and is simply being thorough.");
		d.medium.push_back(combatLoc("Venomspire Tower of Dread", e));
	}
	
	// Medium 6 | Creeping Mollusk
	{
		auto sp1 = spell("Umbral Strike", "The user emerges from creeping darkness and delivers a sudden, silent blow", 6, 3, "Dark");
		auto sp2 = spell("Shadowfall Kick", "The user bursts from the shadows and crashes into the target with a brutal kick", 8, 4, "Dark");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Creeping Mollusk", 22, 2.25, 3, 6, 12, 1.4, spells);
		e->addReward(consumable("Cursed Mollusk Flesh", "A dark, rubbery slab of mollusk meat that carries a faint metallic taste and a whisper of corruption.", 7.5), 50, 3);
		e->addReward(armor("Hood of the Silent Stalker", "A shadow-soaked hood once worn by a lurking predator of the night.", 2, 1), 20, 1);
		e->addReward(armor("Robes of the Silent Stalker", "Tattered robes that swallow light, once belonging to a creature that hunted from darkness.", 3, 2), 20, 1);
		e->addReward(weapon("Whispering Blade", "Knife", "A thin, sinister knife forged for silent kills in dark corners.", 2.5), 10, 1);
		e->setSoulStoneDropChance(11);
		e->setDescription("A low, fast mollusk that clings to the corners of rooms and the edges of vision. It does not charge — it arrives. By the time you hear it move, it has already decided where to strike.");
		d.medium.push_back(combatLoc("Whispering Alley", e));
	}
	
	// Medium 7 | Mudspike Leech
	{
		auto sp1 = chain("Whirling Abyss Dance", "The user twists into a violent spinning assault, striking the target again and again with relentless force", 0.5, 3, 2, 10, "Dark");
		auto sp2 = lifesteal("Thorn of the Bloodleecher", "The user launches a cursed thorn that burrows into the target and siphons their life essence", 6, 4, 70, "Blood");
		auto sp3 = chain("Storm of Mire Spikes", "The user hurls a barrage of jagged mudspikes that rip through flesh like shards of stone", 0.75, 3, 2, 8, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Mudspike Leech", 25, 2.5, 3, 5, 12, 1.4, spells);
		e->addReward(consumable("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10), 50, 3);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->setSoulStoneDropChance(13);
		e->setDescription("A broad, mud-armoured leech whose body is riddled with jagged spikes of compressed filth. It is not elegant. It grinds through opposition through sheer accumulated violence, each blow heavier than the last.");
		d.medium.push_back(combatLoc("The Rotting Mire", e));
	}
	
	// Medium Treasure 1 | The Ironbound Hall Reliquary
	{
		// Reuse Ferric Impact — it's the first medium spell we created
		Spell* ferricImpact = nullptr;
		for (auto& sp : d.spells) {
			if (sp && sp->getName() == "Ferric Impact") { ferricImpact = sp.get(); break; }
		}
		auto loc = treasureLoc("The Ironbound Hall Reliquary", "Rusted Iron Reliquary");
		loc->addReward(armor("Ironshell Faulds", "A wide iron skirt-plate hammered from knight's shell, designed to stop blades that creep low.", 3.5, 4), 45, 1);
		loc->addReward(armor("Ironshell Sabatons", "Boots plated in iron, heavy with purpose.", 2.0, 5), 45, 1);
		loc->addReward(weapon("Ironbound Shell Maul", "Maul", "A two-handed maul built from the layered shell plating of fallen knights. Swings with terrifying momentum.", 4.5), 30, 1);
		if (ferricImpact) loc->addReward(scroll("", ferricImpact), 20, 1);
		loc->setSoulStoneDropChance(15);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Treasure 2 | The Venomspire Tower Alcove
	{
		Spell* vfStrike = nullptr; Spell* blightedJab = nullptr;
		for (auto& sp : d.spells) {
			if (sp && sp->getName() == "Venomous Fang Strike") vfStrike = sp.get();
			if (sp && sp->getName() == "Blighted Jab") blightedJab = sp.get();
		}
		auto loc = treasureLoc("The Venomspire Tower Alcove", "Sealed Alchemist's Cache");
		loc->addReward(weapon("Blighted Fang Dagger", "Knife", "A narrow dagger carved from a venomspire spike, still seeping a dull black residue along its edge.", 3.0), 40, 1);
		loc->addReward(consumable("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak.", 7.5), 55, 2);
		loc->addReward(consumable("Distilled Blight Extract", "A small vial of concentrated poison harvested from venomspire husks. Drinking it burns. Then it heals.", 18.0), 35, 1);
		if (vfStrike) loc->addReward(scroll("", vfStrike, "Tome"), 25, 1);
		if (blightedJab) loc->addReward(scroll("", blightedJab, "Tome"), 25, 1);
		loc->setSoulStoneDropChance(15);
		loc->setXpReward(40);
		d.medium.push_back(loc);
	}
	
	// Medium Heal 1 | Iron Benediction Shrine
	{
		auto loc = healLoc("Iron Chapel Row", "Iron Benediction Shrine");
		d.medium.push_back(loc);
	}
	
	// Medium Heal 2 | Hollow Choir Monument
	{
		auto loc = healLoc("Echoing Stone Square", "Hollow Choir Monument");
		d.medium.push_back(loc);
	}
	
	// Medium Heal 3 | Veinlit Resting Idol
	{
		auto loc = healLoc("Veinlit Passage", "Veinlit Resting Idol");
		d.medium.push_back(loc);
	}
	
	// =============================================
	//  HARD ENEMIES
	// =============================================
	
	// Hard 1 | Bloodcarapace Marauder
	{
		auto sp1 = spell("Iron Cataclysm Fist", "The marauder encases his arm in jagged metal and delivers a devastating strike.", 12, 4, "Physical");
		auto fx1 = hpfx("Hemorrhage", 4, 2, -1.5);
		auto sp2 = statusSpell("Carapace Rend", "Hardened claws tear deep, leaving a brutal bleeding wound.", 6, 2, 70, fx1, "Physical");
		auto sp3 = lifesteal("Crimson Devourer", "A massive tendril lashes out and drains vitality without mercy.", 8, 3, 80, "Blood");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Bloodcarapace Marauder", 40, 4, 5, 4, 12, 1.5, spells);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(weapon("Bloodforged Carapace Mace", "Mace", "A brutal mace forged from dense snail metal.", 5), 25, 1);
		e->addReward(armor("Crimson Carapace Cuirass", "A heavy chestplate forged from blood-hardened snail carapace, scarred by countless battles.", 7, 2), 25, 1);
		e->setSoulStoneDropChance(15);
		e->setDescription("A massive snail in a shell crusted with dried blood, its armour split and reforged so many times it no longer resembles its original shape. It wears every scar like a record of things that tried to stop it.");
		d.hard.push_back(combatLoc("The Crimson Bastion", e));
	}
	
	// Hard 2 | Soulbound Enforcer
	{
		auto sp1 = spell("Soulbreaker Fist", "A crushing strike infused with tormented soul energy.", 10, 3, "Dark");
		auto sp2 = chain("Eternal Soulstorm", "Shattered fragments of fallen souls erupt in a relentless barrage.", 1, 4, 4, 12, "Dark");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Soulbound Enforcer", 42, 4, 5, 5, 12, 1.5, spells);
		e->addReward(consumable("Fractured Soul Core", "A dim, whispering remnant of a warrior consumed by the Enforcer.", 20), 30, 3);
		e->addReward(scroll("", sp1), 10, 1);
		e->addReward(armor("Soulbound Carapace Helm", "A dense snail-metal helm infused with restless spirits.", 4, 1), 30, 1);
		e->addReward(armor("Soulbound Carapace Gauntlets", "Heavy gauntlets pulsing faintly with trapped souls.", 2.5, 3), 30, 1);
		e->setSoulStoneDropChance(16);
		e->setDescription("A towering enforcer bound in chains that don't restrain it — they are part of it, fused into the shell, wrapped around its limbs. The rattling they make is the only warning you get.");
		d.hard.push_back(combatLoc("The Soulbound Bastion", e));
	}
	
	// Hard 3 | Venomspike Assassin
	{
		auto fx1 = hpfx("Black Venom", 3, 5, -2);
		auto sp1 = statusSpell("Venomspike Shot", "The user launches a razor-sharp venomspike infused with deadly toxin", 10, 3, 70, fx1, "Dark");
		auto sp2 = spell("Umbral Jab", "The user melts into shadow, reappearing behind the target to drive a precise strike into their side", 7, 2, "Dark");
		auto sp3 = spell("Umbral Kick", "The user emerges from the darkness and lands a brutal kick before fading back into shadow", 9, 3, "Dark");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Venomspike Assassin", 34, 4, 5, 6, 12, 1.5, spells);
		e->addReward(consumable("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak", 7.5), 50, 3);
		e->addReward(scroll("", sp1), 25, 1);
		e->addReward(scroll("", sp2), 25, 1);
		e->addReward(scroll("", sp3), 25, 1);
		e->addReward(weapon("Venomspike Fangblade", "Knife", "A jagged fang taken from the Venomspike Assassin, still slick with lethal poison", 4), 15, 1);
		e->setSoulStoneDropChance(17);
		e->setDescription("A sleek, hollow-eyed assassin with a shell worn flat and thin from years of pressing into walls. Its venom spines leave marks that keep bleeding after the wound should have closed. It does not linger once the job is done.");
		d.hard.push_back(combatLoc("The Venomshroud Bastion", e));
	}
	
	// Hard 4 | Deathcoil Snail
	{
		auto sp1 = spell("Coiling Charge", "The snail coils into a spinning mass and violently slams into the target", 11, 3, "Physical");
		auto sp2 = chain("Thunderous Spiral", "The creature spins uncontrollably, crashing into the target again and again like a living battering ram", 1.5, 3, 1, 5, "Physical");
		auto sp3 = spell("Dust Vortex", "The snail spins at terrifying speed, unleashing a choking storm of dust and debris", 7, 2, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Deathcoil Snail", 36, 4, 5, 6, 12, 1.5, spells);
		e->addReward(consumable("Deathcoil Snail Meat", "Dense flesh from a violently spinning snail, still trembling with residual force.", 8.5), 50, 3);
		e->addReward(armor("Coilbound Leggings", "Leg armor formed from hardened spiral shell fragments.", 5, 4), 25, 1);
		e->addReward(armor("Coilbound Boots", "Heavy boots reinforced with twisted shell ridges for stability.", 4, 5), 25, 1);
		e->addReward(weapon("Deathcoil Shell Club", "Club", "A brutal club carved from the spiral shell of a Deathcoil Snail.", 4.5), 25, 1);
		e->addReward(scroll("", sp1), 10, 1);
		e->addReward(scroll("", sp2), 10, 1);
		e->addReward(scroll("", sp3), 10, 1);
		e->setSoulStoneDropChance(16);
		e->setDescription("A snail that has coiled its shell so tightly inward that its body is a compressed spiral of dense muscle and ridged shell. When it unwinds and charges, the noise is like something being torn from the ground.");
		d.hard.push_back(combatLoc("The Rolling Bastion", e));
	}
	
	// Hard 5 | Obsidian Champion
	{
		auto sp1 = spell("Obsidian Fist", "The champion cloaks its fist in jagged obsidian and strikes with crushing force", 13, 4, "Physical");
		auto sp2 = chain("Shards of Obsidian", "The champion hurls razor-sharp obsidian shards in a deadly scatter towards the target", 1, 3, 1, 10, "Physical");
		Spell* spells[4] = {sp1, sp2, nullptr, nullptr};
		auto e = enemy("Obsidian Champion", 44, 4, 5, 3, 12, 1.5, spells);
		e->addReward(armor("Obsidian Carapace", "A chestplate formed from dark, tempered obsidian", 7.5, 2), 25, 1);
		e->addReward(armor("Obsidian Gauntlets", "Gauntlets crafted from razor-sharp obsidian shards", 3, 3), 25, 1);
		e->addReward(scroll("", sp1), 10, 1);
		e->addReward(scroll("", sp2), 10, 1);
		e->setSoulStoneDropChance(18);
		e->setDescription("A snail hewn from pure obsidian, its shell smooth and volcanically dark, its fists wrapped in jagged black stone. It does not move fast, but when it strikes the earth shakes under both of you.");
		d.hard.push_back(combatLoc("The Obsidian Bastion", e));
	}
	
	// Hard 6 | Tidal Leech Commander
	{
		auto sp1 = lifesteal("Hydro Drain", "The commander splashes corrosive water, masking tendrils that sap vitality from the target", 12, 4, 75, "Water");
		auto fx1 = hpfx("Hemorrhage", 1, 4, -1.5);
		auto sp2 = statusSpell("Water Spike", "A jet of water pierces the target, causing bleeding", 7, 3, 70, fx1, "Water");
		auto sp3 = chain("Crimson Bubble Barrage", "The commander fires a relentless stream of blood-filled bubbles at the target", 0.5, 3, 1, 16, "Blood");
		Spell* spells[4] = {sp1, sp2, sp3, nullptr};
		auto e = enemy("Tidal Leech Commander", 42, 4, 5, 4, 12, 1.5, spells);
		e->addReward(armor("Coral Carapace", "Chest armor grown from hardened coral, slick with algae", 7, 2), 25, 1);
		e->addReward(armor("Coral Leggings", "Leg armor reinforced with jagged coral formations", 4.5, 4), 25, 1);
		e->addReward(weapon("Coral Club", "Club", "A club carved from dense coral, reinforced by the tides", 4), 25, 1);
		e->addReward(scroll("", sp1), 10, 1);
		e->addReward(scroll("", sp2), 10, 1);
		e->addReward(scroll("", sp3), 10, 1);
		e->setSoulStoneDropChance(20);
		e->setDescription("A commander-class leech swollen with consumed blood, its coral-plated armour grown directly into its flesh. It leads nothing — its title is a warning. Every wound it opens it drinks from.");
		d.hard.push_back(combatLoc("The Coral Bastion", e));
	}
	
	// Hard Treasure 1 | The Crimson Bastion War Vault
	{
		Spell* carapaceRend = nullptr; Spell* crimsonDevourer = nullptr;
		for (auto& sp : d.spells) {
			if (sp && sp->getName() == "Carapace Rend") carapaceRend = sp.get();
			if (sp && sp->getName() == "Crimson Devourer") crimsonDevourer = sp.get();
		}
		auto loc = treasureLoc("The Crimson Bastion War Vault", "Bloodlocked Iron War Chest");
		loc->addReward(armor("Marauder's Bloodforged Helm", "A helmet hammered from blood-hardened carapace, its visor cracked and stained. Still terrifying.", 5.0, 1), 40, 1);
		loc->addReward(armor("Marauder's Crimson Greaves", "Leg armour built from layered blood-carapace, heavy and unrelenting.", 4.5, 4), 40, 1);
		loc->addReward(weapon("Serrated Carapace Cleaver", "Cleaver", "A broad, serrated blade hacked from hardened snail carapace. Not elegant. Deeply effective.", 5.5), 35, 1);
		if (carapaceRend) loc->addReward(scroll("", carapaceRend), 20, 1);
		if (crimsonDevourer) loc->addReward(scroll("", crimsonDevourer), 20, 1);
		loc->setSoulStoneDropChance(20);
		loc->setXpReward(55);
		d.hard.push_back(loc);
	}
	
	// Hard Treasure 2 | The Obsidian Bastion Sealed Chamber
	{
		Spell* obsidianFist = nullptr; Spell* shardsObsidian = nullptr;
		for (auto& sp : d.spells) {
			if (sp && sp->getName() == "Obsidian Fist") obsidianFist = sp.get();
			if (sp && sp->getName() == "Shards of Obsidian") shardsObsidian = sp.get();
		}
		auto loc = treasureLoc("The Obsidian Bastion Sealed Chamber", "Obsidian-Clasped Warlord's Casket");
		loc->addReward(armor("Obsidian Warlord's Helm", "A helmet carved from a single slab of volcanic obsidian, its surface impossibly smooth. Cold to the touch always.", 5.5, 1), 40, 1);
		loc->addReward(armor("Obsidian Warlord's Gauntlets", "Gauntlets of dense obsidian plate, sharp-edged even on the back of the hands.", 3.5, 3), 40, 1);
		loc->addReward(consumable("Obsidian Marrow Draught", "A thick, black liquid rendered from obsidian snail bone. Drinking it feels like swallowing gravel. Then the wounds close.", 22.0), 50, 2);
		if (obsidianFist) loc->addReward(scroll("", obsidianFist), 25, 1);
		if (shardsObsidian) loc->addReward(scroll("", shardsObsidian), 25, 1);
		loc->setSoulStoneDropChance(22);
		loc->setXpReward(55);
		d.hard.push_back(loc);
	}
	
	// Hard Heal 1 | Abyss Reliquary of Renewal
	{
		auto loc = healLoc("Abyssal Reliquary Hall", "Abyss Reliquary of Renewal");
		d.hard.push_back(loc);
	}
	
	// Hard Heal 2 | Crown of Silent Restoration
	{
		auto loc = healLoc("Crowned Silence Chamber", "Crown of Silent Restoration");
		d.hard.push_back(loc);
	}
	
	// =============================================
	//  BOSSES
	// =============================================
	
	// Boss 1 | Claris, the Shell Empress
	{
		auto fx1 = hpfx("Corrosive Venom", 4, 2, -2);
		auto sp1 = statusSpell("Ooze Lash", "The user lashes a writhing mass of blackened slime at the target, corroding flesh and spirit.", 12, 3, 70, fx1, "Dark");
		auto sp2 = spell("Carapace Slam", "The user hurls their jagged shell at the target, crushing with brutal force.", 9, 2, "Physical");
		auto sp3 = statusSpell("Putrescent Cloud", "The user exhales a toxic cloud towards the target, sapping vitality.", 5, 2, 70, fx1, "Dark");
		auto sp4 = spell("Spinning Devastation", "The user spins in their shell at full speed, battering the target mercilessly.", 15, 5, "Physical");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Claris, the Shell Empress", 80, 6, 7, 4, 12, 1.5, spells);
		e->addReward(armor("Claris's Blighted Crown", "A crown throbbing with the corrupt essence of Claris, radiating an unsettling aura.", 5.5, 1), 50, 1);
		e->addReward(weapon("Claris's Scepter of Blackened Muccus", "Scepter", "A ceremonial scepter forged from rare snail-metal, imbued with a sinister, lingering power.", 6.5), 35, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(consumable("Royal Venomflesh", "A dense slab of flesh from a royal poison beast, pulsing with concentrated venom.", 25), 50, 3);
		e->setSoulStoneDropChance(40);
		e->setDescription("A vast, regal snail whose shell has grown into a twisted crown of calcified bone and tarnished metal. Her slime runs black with centuries of corruption. She does not see you as a threat — she sees you as something that wandered into the wrong room.");
		d.boss.push_back(combatLoc("The Blighted Citadel", e));
	}
	
	// Boss 2 | Mathian, the Bloodcarapace
	{
		auto sp1 = lifesteal("Sanguine Sap", "The user rends the target's flesh with a cursed cut and siphons their life force.", 8, 3, 65, "Blood");
		auto sp2 = lifesteal("Crimson Leeching", "The user bites deep, draining the vitality of the target through dark blood magic.", 10, 3, 75, "Blood");
		auto sp3 = spell("Piercing Hemorrhage", "The user slams their hands together, unleashing a pressurized jet of coagulated blood at the target.", 11, 2, "Blood");
		auto fx1 = hpfx("Hemorrhaging", 6, 3, -2.5);
		auto sp4 = statusSpell("Bloody Web", "The user conjures a crimson web of congealed blood, slicing through the target.", 5, 3, 70, fx1, "Blood");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Mathian, the Bloodcarapace", 90, 7, 7, 5, 12, 1.5, spells);
		e->addReward(armor("Mathian's Carapace of Hemogore", "A chestplate molded from the hardened blood of countless victims, pulsating with dark vitality.", 8, 2), 50, 1);
		e->addReward(armor("Mathian's Veinbound Greaves", "Leggings crafted from interlaced veins and coagulated blood, unnervingly alive.", 5, 4), 50, 1);
		e->addReward(weapon("Mathian's Crimson Reaver", "Sword", "A blood-forged blade saturated with the essence of the slain.", 6), 35, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(consumable("Royal Leechflesh", "Rubbery flesh from a royal leech, still warm with the blood it once consumed.", 25), 50, 3);
		e->setSoulStoneDropChance(45);
		e->setDescription("An ancient snail whose shell is built from layers of hardened blood — not its own. Its body pulses with the stolen vitality of dozens. It does not bleed when cut; it simply adjusts, drawing life from you to replace what was lost.");
		d.boss.push_back(combatLoc("The Crimson Bastion", e));
	}
	
	// Boss 3 | Andreas, the Tidal Snail
	{
		auto sp1 = spell("Tidal Crush", "The user summons a towering wave that smashes down upon the target.", 16, 5, "Water");
		auto sp2 = chain("Relentless Drizzle", "The user conjures a stormcloud, raining dagger-like drops upon the target", 0.5, 3, 6, 18, "Physical");
		auto fx1 = hpfx("Drowning", 5, 1, -4);
		auto sp3 = statusSpell("Choking Bubble", "The user traps the target in a suffocating bubble", 5, 3, 65, fx1, "Water");
		auto sp4 = spell("Maelstrom Spin", "The user creates a violent whirlpool that drags the target in", 9, 3, "Water");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Andreas, the Tidal Snail", 100, 7, 7, 3, 12, 1.5, spells);
		e->addReward(armor("Andreas's Carapace of Sunken Bones", "Chestplate forged from the bones of creatures lost to the abyss", 8.5, 2), 50, 1);
		e->addReward(weapon("Andreas's Spear of Drowned Monarchs", "Spear", "A long, coral-encrusted spear once wielded by kings swallowed by the deep", 6), 35, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(consumable("Royal Abyssal Snail Meat", "Cold, rubbery flesh taken from a royal abyssal snail, still smelling of the deep sea.", 25), 50, 3);
		e->setSoulStoneDropChance(45);
		e->setDescription("A colossal snail draped in the barnacled remains of sunken ships, its shell carved by deep-water pressure into something barely recognisable. Water runs from it constantly. The air near it tastes of salt and the dark below the surface.");
		d.boss.push_back(combatLoc("The Sunken Spire", e));
	}
	
	// Boss 4 | Peklorex, the Infernal Slug Tyrant
	{
		auto sp1 = spell("Searing Fist", "The user heats up their fist and crushes the target with molten force", 14, 4, "Fire");
		auto fx1 = hpfx("Cursed Flames", 4, 1, -4);
		auto sp2 = statusSpell("Infernal Breath", "The user exhales a scorching inferno upon the target", 7, 4, 60, fx1, "Fire");
		auto fx2 = hpfx("Boiling Veins", 3, 1, -6);
		auto sp3 = statusSpell("Hellfire Heat", "The user burns the air around the target with demonic fire", 5, 5, 40, fx2, "Fire");
		auto sp4 = chain("Dance of Damnation", "The user writhes possessed by dark flames, striking with ferocity", 2, 3, 2, 5, "Fire");
		Spell* spells[4] = {sp1, sp2, sp3, sp4};
		auto e = enemy("Peklorex, the Infernal Slug Tyrant", 110, 8, 8, 3, 12, 1.6, spells);
		e->addReward(armor("Peklorex's Obsidian Carapace", "A chestplate forged from cursed snail obsidian, worn by the tyrant", 10, 2), 50, 1);
		e->addReward(armor("Peklorex's Gauntlets of Torment", "Gauntlets infused with the essence of fallen victims", 3.5, 3), 50, 1);
		e->addReward(weapon("Peklorex's Soulfire War Axe", "War Axe", "A war axe of snailsidian, carrying the souls of the condemned", 6.5), 35, 1);
		e->addReward(scroll("", sp1), 20, 1);
		e->addReward(scroll("", sp2), 20, 1);
		e->addReward(scroll("", sp3), 20, 1);
		e->addReward(scroll("", sp4), 20, 1);
		e->addReward(consumable("Royal Ember Snail Meat", "Smoldering flesh from a royal heat-dwelling snail, hot enough to burn the unwary.", 25), 50, 3);
		e->setSoulStoneDropChance(50);
		e->setDescription("The tyrant of Snailsville. A slug the size of a siege engine, its body smouldering with contained hellfire that leaks through cracks in its obsidian hide. The ground beneath it chars black. It has been waiting at the end of this place for a long time, and it is not impressed that you made it this far.");
		d.boss.push_back(combatLoc("The Sunken Spire of Torment", e));
	}
	
	return d;
}
