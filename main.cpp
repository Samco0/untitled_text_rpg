#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <filesystem>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "map.h"
#include "weapon.h"
#include "savesystem.h"

using namespace std;
namespace fs = std::filesystem;

int main() {
	srand(time(NULL));
	
	int slot = 1;
	int currentMapIndex = 0;
	string playerName;
	
	// Player starting weapons and spells
	Spell sp1("Trembling Blow", "A weak, unsteady punch that barely fazes the target.", 3, 1, "Physical");
	Spell sp2("Cursed Kick", "A timid kick, shadowed by a faint curse, barely affects the enemy.", 2.5, 1, "Dark");
	Spell* s1[4] = { &sp1, &sp2, nullptr, nullptr};
	Weapon w1("Charred Wooden Tanto", "Sword", "A simple wooden tanto, slightly burnt and brittle, whispering faint dark secrets.", 1.5);
	
	// HP=25, dmg=1.5, speed=4, critChance=15, critValue=1.5
	Player player("", 25, 1.5, 0, 4, 15, 1.5, &w1, s1);
	
	// =============================================
	//  START MENU
	// =============================================
	while (true) {
		system("cls");
		cout << "===============================================" << endl;
		cout << "            untitled text rpg                 " << endl;
		cout << "===============================================" << endl;
		cout << "  1. Step into Shadows   (New Game)"           << endl;
		cout << "  2. Recall Fallen       (Load Game)"          << endl;
		cout << "  3. Burn Memories       (Delete Save)"        << endl;
		cout << "  4. Fade Away           (Exit)"               << endl;
		cout << "===============================================" << endl;
		cout << "Your choice: ";
		
		int choice; cin >> choice;
		
		// ---- NEW GAME ----
		if (choice == 1) {
			system("cls");
			
			// Pick a free slot (or let player choose)
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
			if (slotChoice < 1 || slotChoice > 9){
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
			if (existing.empty()){
				cout << "No graves found. The shadows are empty." << endl;
				system("pause");
				continue;
			}
			
			cout << "===============================================" << endl;
			cout << " Echoes of past adventurers linger:          " << endl;
			cout << "-----------------------------------------------" << endl;
			for (int s : existing){
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
			if (loadGame(currentMapIndex, player, slot)){
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
			if (existing.empty()){
				cout << "No graves to erase." << endl;
				system("pause");
				continue;
			}
			
			cout << "===============================================" << endl;
			cout << " Which grave shall be erased?                " << endl;
			cout << "-----------------------------------------------" << endl;
			for (int s : existing){
				SaveInfo info = getSaveInfo(s);
				cout << "  " << s << ". " << info.playerName
				<< "  (level " << info.level << ")" << endl;
			}
			cout << "===============================================" << endl;
			cout << "Slot (0 to cancel): ";
			int delChoice; cin >> delChoice;
			
			if (delChoice == 0){
				cout << "Returning..." << endl;
				system("pause");
				continue;
			}
			
			SaveInfo info = getSaveInfo(delChoice);
			if (!info.exists){
				cout << "That grave is already empty." << endl;
			} else {
				cout << "Erase " << info.playerName << " forever? (1 = yes): ";
				int confirm; cin >> confirm;
				if (confirm == 1){
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
	
	// Save the starting state right before the adventure begins
	saveGame(currentMapIndex, player, slot);
	
	//Creating enemies and maps
	
	// =============================================
	//  SNAILSVILLE
	// =============================================
	
	//Snailsville | Easy Enemy no.1 | Sputter the Smith
	// Easy 1: Slow (spd 3), player always strikes first. Low HP, minimal attrition.
	// Target: ~5-6 rounds fight, player takes ~6-8 total damage
	Spell easy_snailsville_enemy1_spell1("Cracked Hammer", "The user shapes a jagged, darkened hammer from twisted metal and strikes the target's skull with a resonant clang.", 4, 3, "Physical");
	Spell* easy_snailsville_enemy1_spells[4] = {&easy_snailsville_enemy1_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy1("Sputter the Smith", 14, 1.5, 1, 3, 10, 1.3, easy_snailsville_enemy1_spells);
	Consumable easy_snailsville_enemy1_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Scroll easy_snailsville_enemy1_drop2("Scroll of Cracked Hammer", &easy_snailsville_enemy1_spell1);
	Weapon easy_snailsville_enemy1_drop3("Blackened Shell Sword", "Sword", "Forged from shell and blackened iron, it hums faintly with a restless energy.", 2.5);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop1, 60, 3);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop2, 25, 1);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop3, 15, 1);
	easy_snailsville_enemy1.setSoulStoneDropChance(5);
	easy_snailsville_enemy1.setDescription("A hunched snail-smith blackened by years of forge-smoke, its shell cracked and scorched. It swings a hammer with the slow patience of something that has never known hurry.");
	CombatLocation easy_snailsville_clocation1("The Snailsmith's Forge", &easy_snailsville_enemy1);
	vector<Location*> easy_snailsville_locations = {&easy_snailsville_clocation1};
	
//Snailsville | Easy Enemy no.2 | Shellcarver Apprentice
	// Easy 2: slightly more HP, pebble barrage toned down
	Spell easy_snailsville_enemy2_spell1("Shattered Rock", "The user hurls a jagged fragment of stone, cracking it against the target with a sharp impact.", 5, 3, "Physical");
	ChainingSpell easy_snailsville_enemy2_spell2("Pebble Barrage", "A swarm of sharp, unnatural pebbles erupts from the user's hands, cutting at the target relentlessly.", 0.75, 4, 3, 8, "Physical");
	Spell* easy_snailsville_enemy2_spells[4] = {&easy_snailsville_enemy2_spell1, &easy_snailsville_enemy2_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy2("Shellcarver Apprentice", 15, 1.75, 2, 3, 10, 1.3, easy_snailsville_enemy2_spells);
	Consumable easy_snailsville_enemy2_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Scroll easy_snailsville_enemy2_drop2("Scroll of Shattered Rock", &easy_snailsville_enemy2_spell1);
	Scroll easy_snailsville_enemy2_drop3("Scroll of Pebble Barrage", &easy_snailsville_enemy2_spell2);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop1, 65, 3);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop2, 20, 1);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop3, 15, 1);
	easy_snailsville_enemy2.setSoulStoneDropChance(5);
	easy_snailsville_enemy2.setDescription("A young apprentice still learning the cruel craft, its soft shell barely hardened. It flings stones with frantic, twitching hands — not skilled, but relentless.");
	CombatLocation easy_snailsville_clocation2("A Pile of Rocks..?", &easy_snailsville_enemy2);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation2);
	
//Snailsville | Easy Enemy no.3 | Glimmering Courier
	// Easy 3: fast and frail, low spell damage
	Spell easy_snailsville_enemy3_spell1("Blinding Flash", "The user emits a sudden, piercing light that sears the target's eyes, leaving a lingering sting.", 3, 2, "Light");
	Spell* easy_snailsville_enemy3_spells[4] = {&easy_snailsville_enemy3_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy3("Glimmering Courier", 9, 1.25, 2, 7, 12, 1.4, easy_snailsville_enemy3_spells);
	Consumable easy_snailsville_enemy3_drop1("Glimmering Snail Flesh", "A chunk of snail flesh, glinting faintly as if it holds a strange, inner light.", 10);
	Scroll easy_snailsville_enemy3_drop2("Scroll of Blinding Flash", &easy_snailsville_enemy3_spell1);
	easy_snailsville_enemy3.addReward(&easy_snailsville_enemy3_drop1, 75, 3);
	easy_snailsville_enemy3.addReward(&easy_snailsville_enemy3_drop2, 25, 1);
	easy_snailsville_enemy3.setSoulStoneDropChance(6);
	easy_snailsville_enemy3.setDescription("A slight, swift snail whose shell catches the light with an eerie inner glow. It darts with unsettling speed, the light from its body flickering like a candle about to die.");
	CombatLocation easy_snailsville_clocation3("The Lighthouse", &easy_snailsville_enemy3);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation3);
	
//Snailsville | Easy Enemy no.4 | Herbalist Snail
	// Easy 4: very fast, low hp, moderate spells
	Spell easy_snailsville_enemy4_spell1("Petal Maelstrom", "The user summons a spiraling storm of brittle, blackened petals that slice at anything in its path.", 4, 3, "Nature");
	Spell easy_snailsville_enemy4_spell2("Thorn Lash", "The user lashes thorned vines at the target, tearing through flesh and armor alike.", 3, 2, "Nature");
	Spell* easy_snailsville_enemy4_spells[4] = {&easy_snailsville_enemy4_spell1, &easy_snailsville_enemy4_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy4("Herbalist Snail", 12, 1.5, 1, 5, 10, 1.25, easy_snailsville_enemy4_spells);
	Consumable easy_snailsville_enemy4_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Consumable easy_snailsville_enemy4_drop2("Withered Rose", "A decayed, blackened rose oozing a faint, sticky residue.", 12.5);
	Scroll easy_snailsville_enemy4_drop3("Scroll of Petal Maelstrom", &easy_snailsville_enemy4_spell1);
	Weapon easy_snailsville_enemy4_drop4("Thorned Vine Whip", "Whip", "A whip braided from thorned vines, dark and twisted, leaving deep scratches.", 2);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop1, 50, 3);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop2, 30, 1);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop3, 15, 1);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop4, 5, 1);
	easy_snailsville_enemy4.setSoulStoneDropChance(5);
	easy_snailsville_enemy4.setDescription("A gaunt snail draped in withered vines and rotting petals. It tends to blackened herbs with a tenderness that makes its violence feel like a betrayal.");
	CombatLocation easy_snailsville_clocation4("The Greenhouse", &easy_snailsville_enemy4);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation4);
	
//Snailsville | Easy Enemy no.5 | Lampbearer Snail
	// Easy 5: moderate hp, Waxstorm hits less hard
	Spell easy_snailsville_enemy5_spell1("Searing Gleam", "The user unleashes a piercing flash of corrupted light, scorching eyes and leaving a lingering daze.", 4, 3, "Fire");
	ChainingSpell easy_snailsville_enemy5_spell2("Waxstorm", "The user trembles violently, raining molten wax shards that stick and burn, scalding flesh and armor alike.", 0.5, 4, 3, 10, "Fire");
	Spell* easy_snailsville_enemy5_spells[4] = {&easy_snailsville_enemy5_spell1, &easy_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy5("Lampbearer Snail", 14, 1.75, 1, 4, 10, 1.25, easy_snailsville_enemy5_spells);
	Consumable easy_snailsville_enemy5_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Weapon easy_snailsville_enemy5_drop2("Slimebound Lantern", "Lamp", "A warped lantern covered in snail slime; its flickering light dances unnaturally across the walls.", 1.5);
	Scroll easy_snailsville_enemy5_drop3("Scroll of Waxstorm", &easy_snailsville_enemy5_spell2);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop1, 60, 3);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop2, 30, 1);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop3, 10, 1);
	easy_snailsville_enemy5.setSoulStoneDropChance(6);
	easy_snailsville_enemy5.setDescription("A broad snail bearing a warped lantern fused into its shell, leaking pale light and the stench of burnt wax. The flame flickers with its breath, as if sharing one slow, shared life.");
	CombatLocation easy_snailsville_clocation5("The Lampbearer's Lair", &easy_snailsville_enemy5);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation5);
	
//Snailsville | Easy Enemy no.6 | Tavern Worker Snail
	// Easy 6: tankiest easy, but spells toned way down — Barrel Smash was a one-shot threat
	Spell easy_snailsville_enemy6_spell1("Barrel Smash", "The snail conjures a warped, oozing barrel and hurls it at full speed, splintering whatever it hits.", 6, 4, "Physical");
	Spell easy_snailsville_enemy6_spell2("Piercing Brew", "The snail slaps its hands together, forming a condensed stream of fermented sludge that pierces the target.", 4, 3, "Dark");
	Spell easy_snailsville_enemy6_spell3("Shattered Plate", "A jagged plate materializes in the snail's hand and flies like a deadly disc at the target.", 3, 2, "Physical");
	Spell* easy_snailsville_enemy6_spells[4] = {&easy_snailsville_enemy6_spell1, &easy_snailsville_enemy6_spell2, &easy_snailsville_enemy6_spell3, nullptr};
	Enemy easy_snailsville_enemy6("Tavern Worker Snail", 18, 2, 3, 3, 10, 1.25, easy_snailsville_enemy6_spells);
	Consumable easy_snailsville_enemy6_drop1("Muccus Ale", "A thick, murky brew that sloshes with unsettling bubbles; not for the faint-hearted.", 12.5);
	Consumable easy_snailsville_enemy6_drop2("Slime Wine", "A viscous, dark alcoholic concoction that burns like molten tar.", 15);
	Scroll easy_snailsville_enemy6_drop3("Scroll of Barrel Smash", &easy_snailsville_enemy6_spell1);
	Scroll easy_snailsville_enemy6_drop4("Scroll of Piercing Brew", &easy_snailsville_enemy6_spell2);
	Scroll easy_snailsville_enemy6_drop5("Scroll of Shattered Plate", &easy_snailsville_enemy6_spell3);
	easy_snailsville_enemy6.addReward(&easy_snailsville_enemy6_drop1, 35, 2);
	easy_snailsville_enemy6.addReward(&easy_snailsville_enemy6_drop2, 25, 2);
	easy_snailsville_enemy6.addReward(&easy_snailsville_enemy6_drop3, 20, 1);
	easy_snailsville_enemy6.addReward(&easy_snailsville_enemy6_drop4, 10, 1);
	easy_snailsville_enemy6.addReward(&easy_snailsville_enemy6_drop5, 10, 1);
	easy_snailsville_enemy6.setSoulStoneDropChance(7);
	easy_snailsville_enemy6.setDescription("A thick-set tavern snail reeking of muccus brew and stale violence. The apron it wears is stained in colours that are not wine. It fights with the casual brutality of someone who has done this before closing time.");
	CombatLocation easy_snailsville_clocation6("The Rotting Tavern", &easy_snailsville_enemy6);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation6);
	
//Snailsville | Easy Enemy no.7 | Fisher Snail
	// Easy 7: low HP, single weak spell
	Spell easy_snailsville_enemy7_spell1("Mire Bubble", "The snail exhales a swollen, murky bubble of mucus that drifts forward before bursting violently against the target.", 4, 3, "Water");
	Spell* easy_snailsville_enemy7_spells[4] = {&easy_snailsville_enemy7_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy7("Fisher Snail", 13, 1.5, 1, 3, 10, 1.25, easy_snailsville_enemy7_spells);
	Consumable easy_snailsville_enemy7_drop1("Mirelake Catch", "A pale fish dragged from the depths of the mucus lake. Its skin feels wrong to the touch.", 7.5);
	Scroll easy_snailsville_enemy7_drop2("Scroll of Mire Bubble", &easy_snailsville_enemy7_spell1);
	Weapon easy_snailsville_enemy7_drop3("Mirebound Fishing Rod", "Fishing Rod", "A warped rod stiffened by dried slime and lake residue. It smells faintly of rot.", 1.75);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop1, 50, 3);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop2, 30, 1);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop3, 20, 1);
	easy_snailsville_enemy7.setSoulStoneDropChance(5);
	easy_snailsville_enemy7.setDescription("A pale, wiry snail crouched at the edge of the mucus lake, fishing rod still clutched in hand. Its eyes are glassy and far away, as if whatever it was waiting to catch already took something from it.");
	CombatLocation easy_snailsville_clocation7("The Mirelake Hut", &easy_snailsville_enemy7);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation7);
	
//Snailsville | Easy Enemy no.8 | Mud Leech
	// Easy 8: lifesteal enemy — reduce to 60% steal, lower base damage
	LifeStealSpell easy_snailsville_enemy8_spell1("Sanguine Leech", "A writhing, vein-thick tendril lashes out and drains vitality straight from the victim's veins.", 6, 4, 60, "Blood");
	Spell easy_snailsville_enemy8_spell2("Grave Mud Slam", "A hulking mass of grave-cold mud forms into a hand and crashes down upon the target.", 4, 3, "Physical");
	Spell* easy_snailsville_enemy8_spells[4] = {&easy_snailsville_enemy8_spell1, &easy_snailsville_enemy8_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy8("Mud Leech", 14, 1.75, 2, 4, 10, 1.25, easy_snailsville_enemy8_spells);
	Consumable easy_snailsville_enemy8_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll easy_snailsville_enemy8_drop2("Scroll of Sanguine Leech", &easy_snailsville_enemy8_spell1);
	easy_snailsville_enemy8.addReward(&easy_snailsville_enemy8_drop1, 60, 3);
	easy_snailsville_enemy8.addReward(&easy_snailsville_enemy8_drop2, 40, 1);
	easy_snailsville_enemy8.setSoulStoneDropChance(6);
	easy_snailsville_enemy8.setDescription("A bloated, mud-slicked creature more leech than snail, its body pulsing with stolen warmth. Where it crawls, the earth darkens. Its touch leaves a cold that lingers long after the wound closes.");
	CombatLocation easy_snailsville_clocation8("The Drowned Hollow", &easy_snailsville_enemy8);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation8);
	
//Snailsville | Easy Enemy no.9 | Tiny Rotworm
	// Easy 9: frailest enemy, minimal lifesteal
	LifeStealSpell easy_snailsville_enemy9_spell1("Frail Siphon", "A thin tendril pierces the flesh and steals a whisper of vitality.", 4, 3, 50, "Blood");
	Spell easy_snailsville_enemy9_spell2("Blight Spit", "The worm spits a string of foul, decaying bile at the target.", 3, 2, "Dark");
	Spell* easy_snailsville_enemy9_spells[4] = {&easy_snailsville_enemy9_spell1, &easy_snailsville_enemy9_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy9("Tiny Rotworm", 8, 1.25, 1, 5, 8, 1.2, easy_snailsville_enemy9_spells);
	Consumable easy_snailsville_enemy9_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll easy_snailsville_enemy9_drop2("Scroll of Frail Siphon", &easy_snailsville_enemy9_spell1);
	Scroll easy_snailsville_enemy9_drop3("Scroll of Blight Spit", &easy_snailsville_enemy9_spell2);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop1, 60, 3);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop2, 20, 1);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop3, 20, 1);
	easy_snailsville_enemy9.setSoulStoneDropChance(5);
	easy_snailsville_enemy9.setDescription("A tiny, wretched thing barely the size of a fist, almost pitiable. Its body is translucent enough to see the dark fluid moving inside. It bites like something with nothing left to lose.");
	CombatLocation easy_snailsville_clocation9("The Rotpit Burrow", &easy_snailsville_enemy9);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation9);
	
	// ---- Easy Treasure no.1 | The Forgotten Forge Chest ----
	// Found near the forge district — smith tools, low-tier weapon, basic consumable
	Weapon easy_treasure1_weapon("Soot-Dusted Tack Hammer", "Hammer", "A battered forge hammer still warm from the coals, left behind by a smith who never came back.", 2.0);
	Consumable easy_treasure1_consumable("Forge Marrow", "A dense lump of rendered snail tallow pressed into a waxy cake. Tastes of iron. Keeps you moving.", 10.0);
	Scroll easy_treasure1_scroll("Scroll of Cracked Hammer", &easy_snailsville_enemy1_spell1);
	TreasureLocation easy_snailsville_tlocation1("The Snailsmith's Backroom", "Soot-Stained Lockbox");
	easy_snailsville_tlocation1.addReward(&easy_treasure1_weapon, 50, 1);
	easy_snailsville_tlocation1.addReward(&easy_treasure1_consumable, 70, 2);
	easy_snailsville_tlocation1.addReward(&easy_treasure1_scroll, 30, 1);
	easy_snailsville_tlocation1.setSoulStoneDropChance(8);
	easy_snailsville_tlocation1.setXpReward(20);
	easy_snailsville_locations.push_back(&easy_snailsville_tlocation1);
	
	// ---- Easy Treasure no.2 | The Glimmering Courier's Satchel ----
	// A courier's dropped bag — light armour scraps, a blinding scroll, a restorative
	Armor easy_treasure2_armor("Courier's Wax-Sealed Cap", "A light cap lacquered in hardened snail wax, faintly luminescent.", 1.0, 1);
	Consumable easy_treasure2_consumable1("Glimmer Salve", "A pale, faintly glowing ointment smeared from Glimmering Courier flesh. Closes wounds without asking questions.", 12.0);
	Scroll easy_treasure2_scroll("Scroll of Blinding Flash", &easy_snailsville_enemy3_spell1);
	TreasureLocation easy_snailsville_tlocation2("The Abandoned Post Route", "Dropped Courier Satchel");
	easy_snailsville_tlocation2.addReward(&easy_treasure2_armor, 40, 1);
	easy_snailsville_tlocation2.addReward(&easy_treasure2_consumable1, 65, 2);
	easy_snailsville_tlocation2.addReward(&easy_treasure2_scroll, 25, 1);
	easy_snailsville_tlocation2.setSoulStoneDropChance(8);
	easy_snailsville_tlocation2.setXpReward(20);
	easy_snailsville_locations.push_back(&easy_snailsville_tlocation2);
	
	// ---- Easy Treasure no.3 | The Tavern Cellar Stash ----
	// Hidden behind rotting barrels — drinks that actually heal, a plated apron, a tavern brawler spell
	Armor easy_treasure3_armor("Grease-Stained Plate Apron", "A thick apron hammered flat and riveted with shell scraps. Smells of old brew and worse things.", 1.5, 2);
	Consumable easy_treasure3_consumable1("Muccus Ale", "A thick, murky brew that sloshes with unsettling bubbles; not for the faint-hearted.", 12.5);
	Consumable easy_treasure3_consumable2("Slime Wine", "A viscous, dark alcoholic concoction that burns like molten tar.", 15.0);
	Scroll easy_treasure3_scroll("Scroll of Barrel Smash", &easy_snailsville_enemy6_spell1);
	TreasureLocation easy_snailsville_tlocation3("The Rotting Tavern Cellar", "Wax-Sealed Barrel Cache");
	easy_snailsville_tlocation3.addReward(&easy_treasure3_armor, 35, 1);
	easy_snailsville_tlocation3.addReward(&easy_treasure3_consumable1, 60, 2);
	easy_snailsville_tlocation3.addReward(&easy_treasure3_consumable2, 40, 1);
	easy_snailsville_tlocation3.addReward(&easy_treasure3_scroll, 20, 1);
	easy_snailsville_tlocation3.setSoulStoneDropChance(10);
	easy_snailsville_tlocation3.setXpReward(20);
	easy_snailsville_locations.push_back(&easy_snailsville_tlocation3);
	
//Snailsville | Medium Enemy no.1 | Ironshell Knight
	// Medium 1: slow and tanky — lots of rounds means steady HP drain. Speed 3 = player always first.
	// Target: ~8-9 rounds to kill, player takes ~12-14 dmg total
	Spell medium_snailsville_enemy1_spell1("Ferric Impact", "The knight encases his gauntlet in hardened iron and delivers a crushing blow.", 6, 3, "Physical");
	Spell* medium_snailsville_enemy1_spells[4] = {&medium_snailsville_enemy1_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy1("Ironshell Knight", 28, 2.5, 3, 3, 10, 1.4, medium_snailsville_enemy1_spells);
	Armor medium_snailsville_enemy1_drop1("Helm of the Ironshell", "A heavy helmet hammered from layered ironshell plating.", 2.5, 1);
	Armor medium_snailsville_enemy1_drop2("Cuirass of the Ironshell", "A reinforced chestplate forged from the knight's own shell.", 4, 2);
	Armor medium_snailsville_enemy1_drop3("Ironshell Gauntlets", "Gauntlets of dense metal plating, still scarred from battle.", 1.5, 3);
	Armor medium_snailsville_enemy1_drop4("Ironshell Greaves", "Leg armor shaped from hardened shell segments.", 3, 4);
	Armor medium_snailsville_enemy1_drop5("Ironshell Sabatons", "Boots plated in iron, heavy with purpose.", 2, 5);
	Scroll medium_snailsville_enemy1_drop6("Scroll of Ferric Impact", &medium_snailsville_enemy1_spell1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop1, 20, 1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop2, 20, 1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop3, 20, 1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop4, 20, 1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop5, 20, 1);
	medium_snailsville_enemy1.addReward(&medium_snailsville_enemy1_drop6, 10, 1);
	medium_snailsville_enemy1.setSoulStoneDropChance(10);
	medium_snailsville_enemy1.setDescription("A knight sealed inside a shell of layered iron plate, slow as a funeral procession. Its face is hidden entirely behind riveted metal. Whatever it once was has long since rusted over.");
	CombatLocation medium_snailsville_clocation1("The Ironbound Hall", &medium_snailsville_enemy1);
	vector<Location*> medium_snailsville_locations = {&medium_snailsville_clocation1};
	
//Snailsville | Medium Enemy no.2 | Shadow Sentry
	// Medium 2: fast (spd 6), goes first — hits player before player can act. Lower HP to compensate.
	// Gloomblade is strong (7 dmg) but CD3 means it fires rarely. Eclipse Barrage avg 3×1=3 dmg.
	Spell medium_snailsville_enemy2_spell1("Umbral Kick", "A swift kick wreathed in condensed shadow energy.", 5, 2, "Dark");
	Spell medium_snailsville_enemy2_spell2("Gloomblade", "A blade of hardened darkness forms and cleaves through the target.", 7, 3, "Dark");
	ChainingSpell medium_snailsville_enemy2_spell3("Eclipse Barrage", "Weapons fall aside as the sentry's fists become void-shrouded, striking in relentless succession.", 1, 3, 1, 5, "Dark");
	Spell* medium_snailsville_enemy2_spells[4] = {&medium_snailsville_enemy2_spell1, &medium_snailsville_enemy2_spell2, &medium_snailsville_enemy2_spell3, nullptr};
	Enemy medium_snailsville_enemy2("Shadow Sentry", 24, 2.5, 3, 6, 12, 1.4, medium_snailsville_enemy2_spells);
	Consumable medium_snailsville_enemy2_drop1("Condensed Shadow Flesh", "Cold, elastic meat infused with lingering darkness.", 15);
	Scroll medium_snailsville_enemy2_drop2("Scroll of Umbral Kick", &medium_snailsville_enemy2_spell1);
	Scroll medium_snailsville_enemy2_drop3("Scroll of Gloomblade", &medium_snailsville_enemy2_spell2);
	Scroll medium_snailsville_enemy2_drop4("Scroll of Eclipse Barrage", &medium_snailsville_enemy2_spell3);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop1, 55, 3);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop2, 15, 1);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop3, 15, 1);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop4, 15, 1);
	medium_snailsville_enemy2.setSoulStoneDropChance(11);
	medium_snailsville_enemy2.setDescription("A lean sentry wrapped in shadow like a second skin, barely visible until it moves. Its eyes are the only thing that catches light — two pale embers hovering in the dark where a face should be.");
	CombatLocation medium_snailsville_clocation2("The Shaded Hollow", &medium_snailsville_enemy2);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation2);
	
	
//Snailsville | Medium Enemy no.3 | Leech Scout
	// Medium 3: speed 5, goes first. Lifesteal means it regen ~5 HP per spell cast. Keep HP lower so it's killable before it sustains too much.
	LifeStealSpell medium_snailsville_enemy3_spell1("Crimson Siphon Strike", "A brutal tendril-infused blow that tears vitality straight from the veins.", 7, 4, 70, "Blood");
	Spell* medium_snailsville_enemy3_spells[4] = {&medium_snailsville_enemy3_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy3("Leech Scout", 22, 2.5, 3, 5, 12, 1.4, medium_snailsville_enemy3_spells);
	Consumable medium_snailsville_enemy3_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll medium_snailsville_enemy3_drop2("Scroll of Crimson Siphon Strike", &medium_snailsville_enemy3_spell1);
	medium_snailsville_enemy3.addReward(&medium_snailsville_enemy3_drop1, 75, 3);
	medium_snailsville_enemy3.addReward(&medium_snailsville_enemy3_drop2, 25, 1);
	medium_snailsville_enemy3.setSoulStoneDropChance(10);
	medium_snailsville_enemy3.setDescription("A lithe, scarred leech that moves with a hunter's economy, leaving faint trails of crimson where it passes. It does not fight out of hunger. It has simply never learned to stop.");
	CombatLocation medium_snailsville_clocation3("The Drowned Vein-Tunnel", &medium_snailsville_enemy3);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation3);
	
//Snailsville | Medium Enemy no.4 | Obsidian Shell Snail
	// Medium 4: speed 4 = ties player. Both attack together. Sturdy armor-dropper — intended to be a gear check room.
	Spell medium_snailsville_enemy4_spell1("Shadowfist Strike", "The user envelops their claw in living shadow and slams it into the foe, chilling the soul", 6, 3, "Dark");
	Spell* medium_snailsville_enemy4_spells[4] = {&medium_snailsville_enemy4_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy4("Obsidian Shell Snail", 26, 2.5, 3, 4, 10, 1.4, medium_snailsville_enemy4_spells);
	Armor medium_snailsville_enemy4_drop1("Helm of Eternal Night", "A blackened helm that whispers with the voices of the damned", 2.5, 1);
	Armor medium_snailsville_enemy4_drop2("Cursed Carapace", "An obsidian chestplate etched with runes that sap warmth from the wearer", 4, 2);
	Armor medium_snailsville_enemy4_drop3("Gauntlets of Umbral Grip", "Shadow-forged gauntlets that chill to the touch", 1.5, 3);
	Armor medium_snailsville_enemy4_drop4("Leggings of the Hollow Shell", "Dark leggings that seem to absorb light around them", 3, 4);
	Armor medium_snailsville_enemy4_drop5("Boots of the Blackened Trail", "Boots that leave faint trails of shadow behind with every step", 2, 5);
	Scroll medium_snailsville_enemy4_drop6("Tome of Shadowfist Strike", &medium_snailsville_enemy4_spell1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop1, 20, 1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop2, 20, 1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop3, 20, 1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop4, 20, 1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop5, 20, 1);
	medium_snailsville_enemy4.addReward(&medium_snailsville_enemy4_drop6, 10, 1);
	medium_snailsville_enemy4.setSoulStoneDropChance(12);
	medium_snailsville_enemy4.setDescription("A snail encased in obsidian shell so dark it seems to swallow the light around it. Standing near it feels like standing at the edge of something you cannot name. The cold it radiates is not temperature.");
	CombatLocation medium_snailsville_clocation4("Obsidianbound Hall of Whispers", &medium_snailsville_enemy4);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation4);
	
//Snailsville | Medium Enemy no.5 | Venomspire Snail
	// Medium 5: DoT threat — Corrosive Venom (-1/tick × 5 ticks = -5 HP over time) is the real danger.
	// Keep base dmg low since DoT adds pressure. Speed 4 = ties player.
	HpStatusEffect medium_snailsville_enemy5_status1("Corrosive Venom", 5, 1, -1);
	StatusEffectSpell medium_snailsville_enemy5_spell1("Venomous Fang Strike", "A fang coated in a writhing black venom tears into the target, burning flesh and soul alike", 6, 4, 70, &medium_snailsville_enemy5_status1, "Dark");
	HpStatusEffect medium_snailsville_enemy5_status2("Blight Poison", 3, 1, -0.75);
	StatusEffectSpell medium_snailsville_enemy5_spell2("Blighted Jab", "A quick strike that injects a creeping poison, leaving the target weakened and shivering", 4, 3, 70, &medium_snailsville_enemy5_status2, "Dark");
	Spell* medium_snailsville_enemy5_spells[4] = {&medium_snailsville_enemy5_spell1, &medium_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy medium_snailsville_enemy5("Venomspire Snail", 24, 2, 3, 4, 12, 1.4, medium_snailsville_enemy5_spells);
	Consumable medium_snailsville_enemy5_drop1("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak", 7.5);
	Scroll medium_snailsville_enemy5_drop2("Tome of Venomous Fang Strike", &medium_snailsville_enemy5_spell1);
	Scroll medium_snailsville_enemy5_drop3("Tome of Blighted Jab", &medium_snailsville_enemy5_spell2);
	Weapon medium_snailsville_enemy5_drop4("Shard of Venomspire Shell", "Knife", "A jagged shard from the snail's shell, still pulsing with dark energy", 2);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop1, 50, 3);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop2, 20, 1);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop3, 20, 1);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop4, 10, 1);
	medium_snailsville_enemy5.setSoulStoneDropChance(10);
	medium_snailsville_enemy5.setDescription("A tall, needle-shelled snail bristling with hollow spines that weep black venom in steady drops. It moves with quiet deliberation, as if it already knows the outcome and is simply being thorough.");
	CombatLocation medium_snailsville_clocation5("Venomspire Tower of Dread", &medium_snailsville_enemy5);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation5);
	
//Snailsville | Medium Enemy no.6 | Creeping Mollusk
	// Medium 6: fast (spd 6), goes first. Shadowfall Kick (8 dmg, CD4) is a meaningful spike every few rounds. Lower base dmg to compensate for going first.
	Spell medium_snailsville_enemy6_spell1("Umbral Strike", "The user emerges from creeping darkness and delivers a sudden, silent blow", 6, 3, "Dark");
	Spell medium_snailsville_enemy6_spell2("Shadowfall Kick", "The user bursts from the shadows and crashes into the target with a brutal kick", 8, 4, "Dark");
	Spell* medium_snailsville_enemy6_spells[4] = {&medium_snailsville_enemy6_spell1, &medium_snailsville_enemy6_spell2, nullptr, nullptr};
	Enemy medium_snailsville_enemy6("Creeping Mollusk", 22, 2.25, 3, 6, 12, 1.4, medium_snailsville_enemy6_spells);
	Consumable medium_snailsville_enemy6_drop1("Cursed Mollusk Flesh", "A dark, rubbery slab of mollusk meat that carries a faint metallic taste and a whisper of corruption.", 7.5);
	Armor medium_snailsville_enemy6_drop2("Hood of the Silent Stalker", "A shadow-soaked hood once worn by a lurking predator of the night.", 2, 1);
	Armor medium_snailsville_enemy6_drop3("Robes of the Silent Stalker", "Tattered robes that swallow light, once belonging to a creature that hunted from darkness.", 3, 2);
	Weapon medium_snailsville_enemy6_drop4("Whispering Blade", "Knife", "A thin, sinister knife forged for silent kills in dark corners.", 2.5);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop1, 50, 3);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop2, 20, 1);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop3, 20, 1);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop4, 10, 1);
	medium_snailsville_enemy6.setSoulStoneDropChance(11);
	medium_snailsville_enemy6.setDescription("A low, fast mollusk that clings to the corners of rooms and the edges of vision. It does not charge — it arrives. By the time you hear it move, it has already decided where to strike.");
	CombatLocation medium_snailsville_clocation6("Whispering Alley", &medium_snailsville_enemy6);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation6);
	
//Snailsville | Medium Enemy no.7 | Mudspike Leech
	// Medium 7: hardest medium — chaining + lifesteal + another chain. Speed 5 goes first.
	// Whirling Dance avg 6 hits × 0.5 = 3 dmg. Storm avg 5 × 0.75 = 3.75. Thorn 7 dmg + 70% steal.
	// Punishing but killable in ~8-10 rounds if player plays well.
	ChainingSpell medium_snailsville_enemy7_spell1("Whirling Abyss Dance", "The user twists into a violent spinning assault, striking the target again and again with relentless force", 0.5, 3, 2, 10, "Dark");
	LifeStealSpell medium_snailsville_enemy7_spell2("Thorn of the Bloodleecher", "The user launches a cursed thorn that burrows into the target and siphons their life essence", 6, 4, 70, "Blood");
	ChainingSpell medium_snailsville_enemy7_spell3("Storm of Mire Spikes", "The user hurls a barrage of jagged mudspikes that rip through flesh like shards of stone", 0.75, 3, 2, 8, "Physical");
	Spell* medium_snailsville_enemy7_spells[4] = {&medium_snailsville_enemy7_spell1, &medium_snailsville_enemy7_spell2, &medium_snailsville_enemy7_spell3, nullptr};
	Enemy medium_snailsville_enemy7("Mudspike Leech", 25, 2.5, 3, 5, 12, 1.4, medium_snailsville_enemy7_spells);
	Consumable medium_snailsville_enemy7_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll medium_snailsville_enemy7_drop2("Scroll of Whirling Abyss Dance", &medium_snailsville_enemy7_spell1);
	Scroll medium_snailsville_enemy7_drop3("Scroll of Thorn of the Bloodleecher", &medium_snailsville_enemy7_spell2);
	Scroll medium_snailsville_enemy7_drop4("Scroll of Storm of Mire Spikes", &medium_snailsville_enemy7_spell3);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop1, 50, 3);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop2, 20, 1);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop3, 20, 1);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop4, 20, 1);
	medium_snailsville_enemy7.setSoulStoneDropChance(13);
	medium_snailsville_enemy7.setDescription("A broad, mud-armoured leech whose body is riddled with jagged spikes of compressed filth. It is not elegant. It grinds through opposition through sheer accumulated violence, each blow heavier than the last.");
	CombatLocation medium_snailsville_clocation7("The Rotting Mire", &medium_snailsville_enemy7);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation7);
	
	// ---- Medium Treasure no.1 | The Ironbound Reliquary ----
	// Sealed behind a rusted gate in the knight's hall — proper armour pieces, a heavy weapon
	Armor medium_treasure1_armor1("Ironshell Faulds", "A wide iron skirt-plate hammered from knight's shell, designed to stop blades that creep low.", 3.5, 4);
	Armor medium_treasure1_armor2("Ironshell Sabatons", "Boots plated in iron, heavy with purpose.", 2.0, 5);
	Weapon medium_treasure1_weapon("Ironbound Shell Maul", "Maul", "A two-handed maul built from the layered shell plating of fallen knights. Swings with terrifying momentum.", 4.5);
	Scroll medium_treasure1_scroll("Scroll of Ferric Impact", &medium_snailsville_enemy1_spell1);
	TreasureLocation medium_snailsville_tlocation1("The Ironbound Hall Reliquary", "Rusted Iron Reliquary");
	medium_snailsville_tlocation1.addReward(&medium_treasure1_armor1, 45, 1);
	medium_snailsville_tlocation1.addReward(&medium_treasure1_armor2, 45, 1);
	medium_snailsville_tlocation1.addReward(&medium_treasure1_weapon, 30, 1);
	medium_snailsville_tlocation1.addReward(&medium_treasure1_scroll, 20, 1);
	medium_snailsville_tlocation1.setSoulStoneDropChance(15);
	medium_snailsville_tlocation1.setXpReward(40);
	medium_snailsville_locations.push_back(&medium_snailsville_tlocation1);
	
	// ---- Medium Treasure no.2 | The Venomspire Alchemist's Cache ----
	// A hidden shelf carved into the tower wall — poison scrolls, a venomous blade, potent consumables
	Weapon medium_treasure2_weapon("Blighted Fang Dagger", "Knife", "A narrow dagger carved from a venomspire spike, still seeping a dull black residue along its edge.", 3.0);
	Consumable medium_treasure2_consumable1("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak.", 7.5);
	Consumable medium_treasure2_consumable2("Distilled Blight Extract", "A small vial of concentrated poison harvested from venomspire husks. Drinking it burns. Then it heals.", 18.0);
	Scroll medium_treasure2_scroll1("Tome of Venomous Fang Strike", &medium_snailsville_enemy5_spell1);
	Scroll medium_treasure2_scroll2("Tome of Blighted Jab", &medium_snailsville_enemy5_spell2);
	TreasureLocation medium_snailsville_tlocation2("The Venomspire Tower Alcove", "Sealed Alchemist's Cache");
	medium_snailsville_tlocation2.addReward(&medium_treasure2_weapon, 40, 1);
	medium_snailsville_tlocation2.addReward(&medium_treasure2_consumable1, 55, 2);
	medium_snailsville_tlocation2.addReward(&medium_treasure2_consumable2, 35, 1);
	medium_snailsville_tlocation2.addReward(&medium_treasure2_scroll1, 25, 1);
	medium_snailsville_tlocation2.addReward(&medium_treasure2_scroll2, 25, 1);
	medium_snailsville_tlocation2.setSoulStoneDropChance(15);
	medium_snailsville_tlocation2.setXpReward(40);
	medium_snailsville_locations.push_back(&medium_snailsville_tlocation2);
	
//Snailsville | Hard Enemy no.1 | Bloodcarapace Marauder
	// Hard 1: speed 4 = ties player. Strong base dmg = constant pressure. 3 different attack types.
	// Crimson Devourer (8 dmg + 80% lifesteal) means it regen ~6 HP per cast. Must be burst-killed.
	// Target: ~10 rounds, player takes ~18-22 dmg
	Spell hard_snailsville_enemy1_spell1("Iron Cataclysm Fist", "The marauder encases his arm in jagged metal and delivers a devastating strike.", 12, 4, "Physical");
	HpStatusEffect hard_snailsville_enemy1_status1("Hemorrhage", 4, 2, -1.5);
	StatusEffectSpell hard_snailsville_enemy1_spell2("Carapace Rend", "Hardened claws tear deep, leaving a brutal bleeding wound.", 6, 2, 70, &hard_snailsville_enemy1_status1, "Physical");
	LifeStealSpell hard_snailsville_enemy1_spell3("Crimson Devourer", "A massive tendril lashes out and drains vitality without mercy.", 8, 3, 80, "Blood");
	Spell* hard_snailsville_enemy1_spells[4] = {&hard_snailsville_enemy1_spell1, &hard_snailsville_enemy1_spell2, &hard_snailsville_enemy1_spell3, nullptr};
	Enemy hard_snailsville_enemy1("Bloodcarapace Marauder", 40, 4, 5, 4, 12, 1.5, hard_snailsville_enemy1_spells);
	Scroll hard_snailsville_enemy1_drop1("Scroll of Carapace Rend", &hard_snailsville_enemy1_spell2);
	Scroll hard_snailsville_enemy1_drop2("Scroll of Crimson Devourer", &hard_snailsville_enemy1_spell3);
	Weapon hard_snailsville_enemy1_drop3("Bloodforged Carapace Mace", "Mace", "A brutal mace forged from dense snail metal.", 5);
	Armor hard_snailsville_enemy1_drop4("Crimson Carapace Cuirass", "A heavy chestplate forged from blood-hardened snail carapace, scarred by countless battles.", 7, 2);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop1, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop2, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop3, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop4, 25, 1);
	hard_snailsville_enemy1.setSoulStoneDropChance(15);
	hard_snailsville_enemy1.setDescription("A massive snail in a shell crusted with dried blood, its armour split and reforged so many times it no longer resembles its original shape. It wears every scar like a record of things that tried to stop it.");
	CombatLocation hard_snailsville_clocation1("The Crimson Bastion", &hard_snailsville_enemy1);
	vector<Location*> hard_snailsville_locations = {&hard_snailsville_clocation1};
	
//Snailsville | Hard Enemy no.2 | Soulbound Enforcer
	// Hard 2: speed 5, goes first. Long fight (high HP), steady 4 dmg/round = ~16-20 dmg over fight.
	// Eternal Soulstorm avg 8 hits × 1 = 8 dmg — a brutal but honest spike when it fires.
	Spell hard_snailsville_enemy2_spell1("Soulbreaker Fist", "A crushing strike infused with tormented soul energy.", 10, 3, "Dark");
	ChainingSpell hard_snailsville_enemy2_spell2("Eternal Soulstorm", "Shattered fragments of fallen souls erupt in a relentless barrage.", 1, 4, 4, 12, "Dark");
	Spell* hard_snailsville_enemy2_spells[4] = {&hard_snailsville_enemy2_spell1, &hard_snailsville_enemy2_spell2, nullptr, nullptr};
	Enemy hard_snailsville_enemy2("Soulbound Enforcer", 42, 4, 5, 5, 12, 1.5, hard_snailsville_enemy2_spells);
	Consumable hard_snailsville_enemy2_drop1("Fractured Soul Core", "A dim, whispering remnant of a warrior consumed by the Enforcer.", 20);
	Scroll hard_snailsville_enemy2_drop2("Scroll of Soulbreaker Fist", &hard_snailsville_enemy2_spell1);
	Armor hard_snailsville_enemy2_drop3("Soulbound Carapace Helm", "A dense snail-metal helm infused with restless spirits.", 4, 1);
	Armor hard_snailsville_enemy2_drop4("Soulbound Carapace Gauntlets", "Heavy gauntlets pulsing faintly with trapped souls.", 2.5, 3);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop1, 30, 3);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop2, 10, 1);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop3, 30, 1);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop4, 30, 1);
	hard_snailsville_enemy2.setSoulStoneDropChance(16);
	hard_snailsville_enemy2.setDescription("A towering enforcer bound in chains that don't restrain it — they are part of it, fused into the shell, wrapped around its limbs. The rattling they make is the only warning you get.");
	CombatLocation hard_snailsville_clocation2("The Soulbound Bastion", &hard_snailsville_enemy2);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation2);
	
//Snailsville | Hard Enemy no.3 | Venomspike Assassin
	// Hard 3: speed 6, goes first. Fast and punishing — Black Venom DoT (-2/tick × 5 = -10 HP) is devastating if it lands.
	// Lower HP than others = shorter fight (fewer rounds) but each round hurts.
	HpStatusEffect hard_snailsville_enemy3_status1("Black Venom", 3, 5, -2);	
	StatusEffectSpell hard_snailsville_enemy3_spell1("Venomspike Shot", "The user launches a razor-sharp venomspike infused with deadly toxin", 10, 3, 70, &hard_snailsville_enemy3_status1, "Dark");
	Spell hard_snailsville_enemy3_spell2("Umbral Jab", "The user melts into shadow, reappearing behind the target to drive a precise strike into their side", 7, 2, "Dark");
	Spell hard_snailsville_enemy3_spell3("Umbral Kick", "The user emerges from the darkness and lands a brutal kick before fading back into shadow", 9, 3, "Dark");
	Spell* hard_snailsville_enemy3_spells[4] = {&hard_snailsville_enemy3_spell1, &hard_snailsville_enemy3_spell2, &hard_snailsville_enemy3_spell3, nullptr};
	Enemy hard_snailsville_enemy3("Venomspike Assassin", 34, 4, 5, 6, 12, 1.5, hard_snailsville_enemy3_spells);
	Consumable hard_snailsville_enemy3_drop1("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak", 7.5);
	Scroll hard_snailsville_enemy3_drop2("Scroll of Venomspike Shot", &hard_snailsville_enemy3_spell1);
	Scroll hard_snailsville_enemy3_drop3("Scroll of Umbral Jab", &hard_snailsville_enemy3_spell2);
	Scroll hard_snailsville_enemy3_drop4("Scroll of Umbral Kick", &hard_snailsville_enemy3_spell3);
	Weapon hard_snailsville_enemy3_drop5("Venomspike Fangblade", "Knife", "A jagged fang taken from the Venomspike Assassin, still slick with lethal poison", 4);
	hard_snailsville_enemy3.addReward(&hard_snailsville_enemy3_drop1, 50, 3);
	hard_snailsville_enemy3.addReward(&hard_snailsville_enemy3_drop2, 25, 1);
	hard_snailsville_enemy3.addReward(&hard_snailsville_enemy3_drop3, 25, 1);
	hard_snailsville_enemy3.addReward(&hard_snailsville_enemy3_drop4, 25, 1);
	hard_snailsville_enemy3.addReward(&hard_snailsville_enemy3_drop5, 15, 1);
	hard_snailsville_enemy3.setSoulStoneDropChance(17);
	hard_snailsville_enemy3.setDescription("A sleek, hollow-eyed assassin with a shell worn flat and thin from years of pressing into walls. Its venom spines leave marks that keep bleeding after the wound should have closed. It does not linger once the job is done.");
	CombatLocation hard_snailsville_clocation3("The Venomshroud Bastion", &hard_snailsville_enemy3);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation3);
	
//Snailsville | Hard Enemy no.4 | Deathcoil Snail
	// Hard 4: speed 6, goes first. Spinning focus — Thunderous Spiral avg 3×1.5=4.5 dmg, Coiling Charge hits hard.
	// Moderate HP — fast fight, punishing if unlucky with spiral hits.
	Spell hard_snailsville_enemy4_spell1("Coiling Charge", "The snail coils into a spinning mass and violently slams into the target", 11, 3, "Physical");
	ChainingSpell hard_snailsville_enemy4_spell2("Thunderous Spiral", "The creature spins uncontrollably, crashing into the target again and again like a living battering ram", 1.5, 3, 1, 5, "Physical");
	Spell hard_snailsville_enemy4_spell3("Dust Vortex", "The snail spins at terrifying speed, unleashing a choking storm of dust and debris", 7, 2, "Physical");
	Spell* hard_snailsville_enemy4_spells[4] = {&hard_snailsville_enemy4_spell1, &hard_snailsville_enemy4_spell2, &hard_snailsville_enemy4_spell3, nullptr};
	Enemy hard_snailsville_enemy4("Deathcoil Snail", 36, 4, 5, 6, 12, 1.5, hard_snailsville_enemy4_spells);
	Consumable hard_snailsville_enemy4_drop1("Deathcoil Snail Meat", "Dense flesh from a violently spinning snail, still trembling with residual force.", 8.5);
	Armor hard_snailsville_enemy4_drop2("Coilbound Leggings", "Leg armor formed from hardened spiral shell fragments.", 5, 4);
	Armor hard_snailsville_enemy4_drop3("Coilbound Boots", "Heavy boots reinforced with twisted shell ridges for stability.", 4, 5);
	Weapon hard_snailsville_enemy4_drop4("Deathcoil Shell Club", "Club", "A brutal club carved from the spiral shell of a Deathcoil Snail.", 4.5);
	Scroll hard_snailsville_enemy4_drop5("Scroll of Coiling Charge", &hard_snailsville_enemy4_spell1);
	Scroll hard_snailsville_enemy4_drop6("Scroll of Thunderous Spiral", &hard_snailsville_enemy4_spell2);
	Scroll hard_snailsville_enemy4_drop7("Scroll of Dust Vortex", &hard_snailsville_enemy4_spell3);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop1, 50, 3);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop2, 25, 1);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop3, 25, 1);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop4, 25, 1);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop5, 10, 1);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop6, 10, 1);
	hard_snailsville_enemy4.addReward(&hard_snailsville_enemy4_drop7, 10, 1);
	hard_snailsville_enemy4.setSoulStoneDropChance(16);
	hard_snailsville_enemy4.setDescription("A snail that has coiled its shell so tightly inward that its body is a compressed spiral of dense muscle and ridged shell. When it unwinds and charges, the noise is like something being torn from the ground.");
	CombatLocation hard_snailsville_clocation4("The Rolling Bastion", &hard_snailsville_enemy4);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation4);
	
//Snailsville | Hard Enemy no.5 | Obsidian Champion
	// Hard 5: slowest hard enemy (spd 3), player always strikes first. Compensate with high HP and hard-hitting spells.
	// Shards of Obsidian avg 5.5×1=5.5 dmg — a reliable moderate spike. Obsidian Fist (13) is the scary one.
	Spell hard_snailsville_enemy5_spell1("Obsidian Fist", "The champion cloaks its fist in jagged obsidian and strikes with crushing force", 13, 4, "Physical");
	ChainingSpell hard_snailsville_enemy5_spell2("Shards of Obsidian", "The champion hurls razor-sharp obsidian shards in a deadly scatter towards the target", 1, 3, 1, 10, "Physical");
	Spell* hard_snailsville_enemy5_spells[4] = {&hard_snailsville_enemy5_spell1, &hard_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy hard_snailsville_enemy5("Obsidian Champion", 44, 4, 5, 3, 12, 1.5, hard_snailsville_enemy5_spells);
	Armor hard_snailsville_enemy5_drop1("Obsidian Carapace", "A chestplate formed from dark, tempered obsidian", 7.5, 2);
	Armor hard_snailsville_enemy5_drop2("Obsidian Gauntlets", "Gauntlets crafted from razor-sharp obsidian shards", 3, 3);
	Scroll hard_snailsville_enemy5_drop3("Scroll of Obsidian Fist", &hard_snailsville_enemy5_spell1);
	Scroll hard_snailsville_enemy5_drop4("Scroll of Shards of Obsidian", &hard_snailsville_enemy5_spell2);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop1, 25, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop2, 25, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop3, 10, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop4, 10, 1);
	hard_snailsville_enemy5.setSoulStoneDropChance(18);
	hard_snailsville_enemy5.setDescription("A snail hewn from pure obsidian, its shell smooth and volcanically dark, its fists wrapped in jagged black stone. It does not move fast, but when it strikes the earth shakes under both of you.");
	CombatLocation hard_snailsville_clocation5("The Obsidian Bastion", &hard_snailsville_enemy5);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation5);
	
	// Snailsville | Hard Enemy no.6 | Tidal Leech Commander
	// Hard 6: hardest regular enemy. Speed 4 = ties player (both attack simultaneously).
	// 3 spell types: lifesteal, DoT bleed, chaining. Hydro Drain (12 + 75% lifesteal) means it can regen ~9 HP.
	// Must be killed fast before it sustains too much.
	LifeStealSpell hard_snailsville_enemy6_spell1("Hydro Drain", "The commander splashes corrosive water, masking tendrils that sap vitality from the target", 12, 4, 75, "Water");
	HpStatusEffect hard_snailsville_enemy6_status1("Hemorrhage", 1, 4, -1.5);
	StatusEffectSpell hard_snailsville_enemy6_spell2("Water Spike", "A jet of water pierces the target, causing bleeding", 7, 3, 70, &hard_snailsville_enemy6_status1, "Water");
	ChainingSpell hard_snailsville_enemy6_spell3("Crimson Bubble Barrage", "The commander fires a relentless stream of blood-filled bubbles at the target", 0.5, 3, 1, 16, "Blood");
	Spell* hard_snailsville_enemy6_spells[4] = {&hard_snailsville_enemy6_spell1, &hard_snailsville_enemy6_spell2, &hard_snailsville_enemy6_spell3, nullptr};
	Enemy hard_snailsville_enemy6("Tidal Leech Commander", 42, 4, 5, 4, 12, 1.5, hard_snailsville_enemy6_spells);
	Armor hard_snailsville_enemy6_drop1("Coral Carapace", "Chest armor grown from hardened coral, slick with algae", 7, 2);
	Armor hard_snailsville_enemy6_drop2("Coral Leggings", "Leg armor reinforced with jagged coral formations", 4.5, 4);
	Weapon hard_snailsville_enemy6_drop3("Coral Club", "Club", "A club carved from dense coral, reinforced by the tides", 4);
	Scroll hard_snailsville_enemy6_drop4("Scroll of Hydro Drain", &hard_snailsville_enemy6_spell1);
	Scroll hard_snailsville_enemy6_drop5("Scroll of Water Spike", &hard_snailsville_enemy6_spell2);
	Scroll hard_snailsville_enemy6_drop6("Scroll of Crimson Bubble Barrage", &hard_snailsville_enemy6_spell3);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop1, 25, 1);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop2, 25, 1);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop3, 25, 1);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop4, 10, 1);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop5, 10, 1);
	hard_snailsville_enemy6.addReward(&hard_snailsville_enemy6_drop6, 10, 1);
	hard_snailsville_enemy6.setSoulStoneDropChance(20);
	hard_snailsville_enemy6.setDescription("A commander-class leech swollen with consumed blood, its coral-plated armour grown directly into its flesh. It leads nothing — its title is a warning. Every wound it opens it drinks from.");
	CombatLocation hard_snailsville_clocation6("The Coral Bastion", &hard_snailsville_enemy6);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation6);
	
	// ---- Hard Treasure no.1 | The Bloodcarapace War Vault ----
	// A sealed vault behind the Crimson Bastion — finest armour of the hard tier, brutal weapon, rare scrolls
	Armor hard_treasure1_armor1("Marauder's Bloodforged Helm", "A helmet hammered from blood-hardened carapace, its visor cracked and stained. Still terrifying.", 5.0, 1);
	Armor hard_treasure1_armor2("Marauder's Crimson Greaves", "Leg armour built from layered blood-carapace, heavy and unrelenting.", 4.5, 4);
	Weapon hard_treasure1_weapon("Serrated Carapace Cleaver", "Cleaver", "A broad, serrated blade hacked from hardened snail carapace. Not elegant. Deeply effective.", 5.5);
	Scroll hard_treasure1_scroll1("Scroll of Carapace Rend", &hard_snailsville_enemy1_spell2);
	Scroll hard_treasure1_scroll2("Scroll of Crimson Devourer", &hard_snailsville_enemy1_spell3);
	TreasureLocation hard_snailsville_tlocation1("The Crimson Bastion War Vault", "Bloodlocked Iron War Chest");
	hard_snailsville_tlocation1.addReward(&hard_treasure1_armor1, 40, 1);
	hard_snailsville_tlocation1.addReward(&hard_treasure1_armor2, 40, 1);
	hard_snailsville_tlocation1.addReward(&hard_treasure1_weapon, 35, 1);
	hard_snailsville_tlocation1.addReward(&hard_treasure1_scroll1, 20, 1);
	hard_snailsville_tlocation1.addReward(&hard_treasure1_scroll2, 20, 1);
	hard_snailsville_tlocation1.setSoulStoneDropChance(20);
	hard_snailsville_tlocation1.setXpReward(55);
	hard_snailsville_locations.push_back(&hard_snailsville_tlocation1);
	
	// ---- Hard Treasure no.2 | The Obsidian Warlord's Sealed Casket ----
	// The deepest chest in Snailsville's hard tier — heaviest armour, obsidian weaponry, powerful scrolls
	Armor hard_treasure2_armor1("Obsidian Warlord's Helm", "A helmet carved from a single slab of volcanic obsidian, its surface impossibly smooth. Cold to the touch always.", 5.5, 1);
	Armor hard_treasure2_armor2("Obsidian Warlord's Gauntlets", "Gauntlets of dense obsidian plate, sharp-edged even on the back of the hands.", 3.5, 3);
	Consumable hard_treasure2_consumable("Obsidian Marrow Draught", "A thick, black liquid rendered from obsidian snail bone. Drinking it feels like swallowing gravel. Then the wounds close.", 22.0);
	Scroll hard_treasure2_scroll1("Scroll of Obsidian Fist", &hard_snailsville_enemy5_spell1);
	Scroll hard_treasure2_scroll2("Scroll of Shards of Obsidian", &hard_snailsville_enemy5_spell2);
	TreasureLocation hard_snailsville_tlocation2("The Obsidian Bastion Sealed Chamber", "Obsidian-Clasped Warlord's Casket");
	hard_snailsville_tlocation2.addReward(&hard_treasure2_armor1, 40, 1);
	hard_snailsville_tlocation2.addReward(&hard_treasure2_armor2, 40, 1);
	hard_snailsville_tlocation2.addReward(&hard_treasure2_consumable, 50, 2);
	hard_snailsville_tlocation2.addReward(&hard_treasure2_scroll1, 25, 1);
	hard_snailsville_tlocation2.addReward(&hard_treasure2_scroll2, 25, 1);
	hard_snailsville_tlocation2.setSoulStoneDropChance(22);
	hard_snailsville_tlocation2.setXpReward(55);
	hard_snailsville_locations.push_back(&hard_snailsville_tlocation2);
	
	//Snailsville | Boss no.1 | Claris, the Shell Empress
	// Boss 1: Claris — speed 4 (ties player). First boss = a step above hard enemies but not a wall.
	// Spinning Devastation (15 dmg, CD5) fires rarely but is the "oh shit" moment.
	// Corrosive Venom DoT (-2/tick × 4 = -8 HP) from Ooze Lash and Putrescent Cloud stacks dangerously.
	// Base dmg 6 means ~6 HP/round = meaningful sustained pressure over the long fight.
	HpStatusEffect snailsville_boss1_status1("Corrosive Venom", 4, 2, -2);
	StatusEffectSpell snailsville_boss1_spell1("Ooze Lash", "The user lashes a writhing mass of blackened slime at the target, corroding flesh and spirit.", 12, 3, 70, &snailsville_boss1_status1, "Dark");
	Spell snailsville_boss1_spell2("Carapace Slam", "The user hurls their jagged shell at the target, crushing with brutal force.", 9, 2, "Physical");
	StatusEffectSpell snailsville_boss1_spell3("Putrescent Cloud", "The user exhales a toxic cloud towards the target, sapping vitality.", 5, 2, 70, &snailsville_boss1_status1, "Dark");
	Spell snailsville_boss1_spell4("Spinning Devastation", "The user spins in their shell at full speed, battering the target mercilessly.", 15, 5, "Physical");
	Spell* snailsville_boss1_spells[4] = {&snailsville_boss1_spell1, &snailsville_boss1_spell2, &snailsville_boss1_spell3, &snailsville_boss1_spell4};
	Enemy snailsville_boss1("Claris, the Shell Empress", 80, 6, 7, 4, 12, 1.5, snailsville_boss1_spells);
	Armor snailsville_boss1_drop1("Claris's Blighted Crown", "A crown throbbing with the corrupt essence of Claris, radiating an unsettling aura.", 5.5, 1);
	Weapon snailsville_boss1_drop2("Claris's Scepter of Blackened Muccus", "Scepter", "A ceremonial scepter forged from rare snail-metal, imbued with a sinister, lingering power.", 6.5);
	Scroll snailsville_boss1_drop3("Scroll of Ooze Lash", &snailsville_boss1_spell1);
	Scroll snailsville_boss1_drop4("Scroll of Carapace Slam", &snailsville_boss1_spell2);
	Scroll snailsville_boss1_drop5("Scroll of Putrescent Cloud", &snailsville_boss1_spell3);
	Scroll snailsville_boss1_drop6("Scroll of Spinning Devastation", &snailsville_boss1_spell4);
	Consumable snailsville_boss1_drop7("Royal Venomflesh", "A dense slab of flesh from a royal poison beast, pulsing with concentrated venom.", 25);
	snailsville_boss1.addReward(&snailsville_boss1_drop1, 50, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop2, 35, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop3, 20, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop4, 20, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop5, 20, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop6, 20, 1);
	snailsville_boss1.addReward(&snailsville_boss1_drop7, 50, 3);
	snailsville_boss1.setSoulStoneDropChance(40);
	snailsville_boss1.setDescription("A vast, regal snail whose shell has grown into a twisted crown of calcified bone and tarnished metal. Her slime runs black with centuries of corruption. She does not see you as a threat — she sees you as something that wandered into the wrong room.");
	CombatLocation boss_snailsville_clocation1("The Blighted Citadel", &snailsville_boss1);
	vector<Location*> boss_snailsville_locations = {&boss_snailsville_clocation1};
	
	//Snailsville | Boss no.2 | Mathan, the Bloodcarapace
	// Boss 2: Mathian — speed 5, goes first. Primary threat is lifesteal sustain.
	// Sanguine Sap: 8 dmg + 65% steal = 5.2 regen. Crimson Leeching: 10 dmg + 75% steal = 7.5 regen.
	// Player must deal fast burst damage — a war of attrition favors Mathian.
	// Base dmg 7 = serious constant pressure.
	LifeStealSpell snailsville_boss2_spell1("Sanguine Sap", "The user rends the target's flesh with a cursed cut and siphons their life force.", 8, 3, 65, "Blood");
	LifeStealSpell snailsville_boss2_spell2("Crimson Leeching", "The user bites deep, draining the vitality of the target through dark blood magic.", 10, 3, 75, "Blood");
	Spell snailsville_boss2_spell3("Piercing Hemorrhage", "The user slams their hands together, unleashing a pressurized jet of coagulated blood at the target.", 11, 2, "Blood");
	HpStatusEffect snailsville_boss2_status1("Hemorrhaging", 6, 3, 2.5);
	StatusEffectSpell snailsville_boss2_spell4("Bloody Web", "The user conjures a crimson web of congealed blood, slicing through the target.", 5, 3, 70, &snailsville_boss2_status1, "Blood");
	Spell* snailsville_boss2_spells[4] = {&snailsville_boss2_spell1, &snailsville_boss2_spell2, &snailsville_boss2_spell3, &snailsville_boss2_spell4};
	Enemy snailsville_boss2("Mathian, the Bloodcarapace", 90, 7, 7, 5, 12, 1.5, snailsville_boss2_spells);
	Armor snailsville_boss2_drop1("Mathian's Carapace of Hemogore", "A chestplate molded from the hardened blood of countless victims, pulsating with dark vitality.", 8, 2);
	Armor snailsville_boss2_drop2("Mathian's Veinbound Greaves", "Leggings crafted from interlaced veins and coagulated blood, unnervingly alive.", 5, 4);
	Weapon snailsville_boss2_drop3("Mathian's Crimson Reaver", "Sword", "A blood-forged blade saturated with the essence of the slain.", 6);
	Scroll snailsville_boss2_drop4("Scroll of Sanguine Sap", &snailsville_boss2_spell1);
	Scroll snailsville_boss2_drop5("Scroll of Crimson Leeching", &snailsville_boss2_spell2);
	Scroll snailsville_boss2_drop6("Scroll of Piercing Hemorrhage", &snailsville_boss2_spell3);
	Scroll snailsville_boss2_drop7("Scroll of Bloody Web", &snailsville_boss2_spell4);
	Consumable snailsville_boss2_drop8("Royal Leechflesh", "Rubbery flesh from a royal leech, still warm with the blood it once consumed.", 25);
	snailsville_boss2.setDescription("A colossal leech-snail whose shell has been replaced, piece by piece, with hardened carapace scavenged from things it bled dry. It breathes in slow, wet heaves. It smells of iron and old meat. It has fed on enough blood to remember every face it drained.");
	snailsville_boss2.addReward(&snailsville_boss2_drop1, 50, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop2, 50, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop3, 35, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop4, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop5, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop6, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop7, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop8, 50, 3);
	snailsville_boss2.setSoulStoneDropChance(45);
	snailsville_boss2.setDescription("An ancient snail whose shell is built from layers of hardened blood — not its own. Its body pulses with the stolen vitality of dozens. It does not bleed when cut; it simply adjusts, drawing life from you to replace what was lost.");
	CombatLocation boss_snailsville_clocation2("The Crimson Bastion", &snailsville_boss2);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation2);
	
	//Snailsville | Boss no.3 | Andreas, the Tidal Snail
	// Boss 3: Andreas — slowest boss (spd 3), player always strikes first. Compensated by highest base dmg so far.
	// Tidal Crush (16, CD5) fires rarely but represents the danger. Relentless Drizzle avg 12×0.5=6 dmg.
	// Choking Bubble DoT (-4/tick × 5) = -20 HP total if it lands — deadly if not cleansed via consumables.
	// Long fight (100 HP), constant 7 dmg/round bleed.
	Spell snailsville_boss3_spell1("Tidal Crush", "The user summons a towering wave that smashes down upon the target.", 16, 5, "Water");
	ChainingSpell snailsville_boss3_spell2("Relentless Drizzle", "The user conjures a stormcloud, raining dagger-like drops upon the target", 0.5, 3, 6, 18, "Physical");
	HpStatusEffect snailsville_boss3_status1("Drowning", 5, 1, 4);
	StatusEffectSpell snailsville_boss3_spell3("Choking Bubble", "The user traps the target in a suffocating bubble", 5, 3, 65, &snailsville_boss3_status1, "Water");
	Spell snailsville_boss3_spell4("Maelstrom Spin", "The user creates a violent whirlpool that drags the target in", 9, 3, "Water");
	Spell* snailsville_boss3_spells[4] = {&snailsville_boss3_spell1, &snailsville_boss3_spell2, &snailsville_boss3_spell3, &snailsville_boss3_spell4};
	Enemy snailsville_boss3("Andreas, the Tidal Snail", 100, 7, 7, 3, 12, 1.5, snailsville_boss3_spells);
	Armor snailsville_boss3_drop1("Andreas's Carapace of Sunken Bones", "Chestplate forged from the bones of creatures lost to the abyss", 8.5, 2);
	Weapon snailsville_boss3_drop2("Andreas's Spear of Drowned Monarchs", "Spear", "A long, coral-encrusted spear once wielded by kings swallowed by the deep", 6);
	Scroll snailsville_boss3_drop3("Scroll of Tidal Crush", &snailsville_boss3_spell1);
	Scroll snailsville_boss3_drop4("Scroll of Relentless Drizzle", &snailsville_boss3_spell2);
	Scroll snailsville_boss3_drop5("Scroll of Choking Bubble", &snailsville_boss3_spell3);
	Scroll snailsville_boss3_drop6("Scroll of Maelstrom Spin", &snailsville_boss3_spell4);
	Consumable snailsville_boss3_drop7("Royal Abyssal Snail Meat", "Cold, rubbery flesh taken from a royal abyssal snail, still smelling of the deep sea.", 25);
	snailsville_boss3.setDescription("An ancient tidal snail whose shell has calcified into something between a fortress and a reef, encrusted with the bones of the drowned. The water that runs from it never dries. Its presence makes the air taste like depth, like pressure, like something that has never seen the surface.");
	snailsville_boss3.addReward(&snailsville_boss3_drop1, 50, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop2, 35, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop3, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop4, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop5, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop6, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop7, 50, 3);
	snailsville_boss3.setSoulStoneDropChance(45);
	snailsville_boss3.setDescription("A colossal snail draped in the barnacled remains of sunken ships, its shell carved by deep-water pressure into something barely recognisable. Water runs from it constantly. The air near it tastes of salt and the dark below the surface.");
	CombatLocation boss_snailsville_clocation3("The Sunken Spire", &snailsville_boss3);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation3);
	
	//Snailsville | Boss no.4 | Peklorex, the Infernal Slug
	// Boss 4: Peklorex — FINAL BOSS. Speed 3, player goes first but this is the ultimate test of the run.
	// Cursed Flames DoT (-4/tick × 4 = -16 HP) + Boiling Veins DoT (-6/tick × 3 = -18 HP) can stack = 34 HP DoT damage.
	// Both DoTs firing in same fight would be lethal without consumables — intentional final boss pressure.
	// Dance of Damnation avg 3.5×2=7 dmg. Searing Fist (14) is the single biggest direct hit.
	// Player should enter with gear and consumables saved — this is the payoff fight.
	Spell snailsville_boss4_spell1("Searing Fist", "The user heats up their fist and crushes the target with molten force", 14, 4, "Fire");
	HpStatusEffect snailsville_boss4_status1("Cursed Flames", 4, 1, 4);
	StatusEffectSpell snailsville_boss4_spell2("Infernal Breath", "The user exhales a scorching inferno upon the target", 7, 4, 60, &snailsville_boss4_status1, "Fire");
	HpStatusEffect snailsville_boss4_status2("Boiling Veins", 3, 1, 6);
	StatusEffectSpell snailsville_boss4_spell3("Hellfire Heat", "The user burns the air around the target with demonic fire", 5, 5, 40, &snailsville_boss4_status2, "Fire");
	ChainingSpell snailsville_boss4_spell4("Dance of Damnation", "The user writhes possessed by dark flames, striking with ferocity", 2, 3, 2, 5, "Fire");
	Spell* snailsville_boss4_spells[4] = {&snailsville_boss4_spell1, &snailsville_boss4_spell2, &snailsville_boss4_spell3, &snailsville_boss4_spell4};
	Enemy snailsville_boss4("Peklorex, the Infernal Slug Tyrant", 110, 8, 8, 3, 12, 1.6, snailsville_boss4_spells);
	Armor snailsville_boss4_drop1("Peklorex's Obsidian Carapace", "A chestplate forged from cursed snail obsidian, worn by the tyrant", 10, 2);
	Armor snailsville_boss4_drop2("Peklorex's Gauntlets of Torment", "Gauntlets infused with the essence of fallen victims", 3.5, 3);
	Weapon snailsville_boss4_drop3("Peklorex's Soulfire War Axe", "War Axe", "A war axe of snailsidian, carrying the souls of the condemned", 6.5);
	Scroll snailsville_boss4_drop4("Scroll of Searing Fist", &snailsville_boss4_spell1);
	Scroll snailsville_boss4_drop5("Scroll of Infernal Breath", &snailsville_boss4_spell2);
	Scroll snailsville_boss4_drop6("Scroll of Hellfire Heat", &snailsville_boss4_spell3);
	Scroll snailsville_boss4_drop7("Scroll of Dance of Damnation", &snailsville_boss4_spell4);
	Consumable snailsville_boss4_drop8("Royal Ember Snail Meat", "Smoldering flesh from a royal heat-dwelling snail, hot enough to burn the unwary.", 25);
	snailsville_boss4.setDescription("A slug of monstrous size, its boneless body armoured in cursed obsidian plates fused to flesh by heat and hatred. Flames bleed from the cracks between them. The air around it shimmers. It does not roar, does not posture — it simply turns toward you with the unhurried certainty of something that has never once lost.");
	snailsville_boss4.addReward(&snailsville_boss4_drop1, 50, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop2, 50, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop3, 35, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop4, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop5, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop6, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop7, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop8, 50, 3);
	snailsville_boss4.setSoulStoneDropChance(50);
	snailsville_boss4.setDescription("The tyrant of Snailsville. A slug the size of a siege engine, its body smouldering with contained hellfire that leaks through cracks in its obsidian hide. The ground beneath it chars black. It has been waiting at the end of this place for a long time, and it is not impressed that you made it this far.");
	CombatLocation boss_snailsville_clocation4("The Sunken Spire of Torment", &snailsville_boss4);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation4);
	
	// =============================================
	//  RUSTHEAP
	// =============================================
	
	//Rustheap | Easy Enemy no. 1 | Gritclaw the Hollow
	Spell easy_rustheap_enemy1_spell1("Corroded Junk Throw", "The user forms a coorroded piece of scrap and casts it towards the target", 14, 3, "Metal");
	Spell easy_rustheap_enemy1_spell2("Corroded junk claw", "The user forms a coorroded scrap claw and lunges towards the enemy", 12, 2, "Metal");
	Spell* easy_rustheap_enemy1_spells[4] = {&easy_rustheap_enemy1_spell1, &easy_rustheap_enemy1_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy1("Gritclaw, the Hollow", 50, 5, 9, 6, 15, 1.5, easy_rustheap_enemy1_spells);
	easy_rustheap_enemy1.setSoulStoneDropChance(6);
	easy_rustheap_enemy1.setDescription("A malnourished young raccoon, barely surviving on the lower heaps. Throws corroded junk with trembling claws — desperate and feral, not yet broken enough to stop fighting.");
	
	// MAP INITIALIZATION
	
	// Create map instance and pass player reference to it
	// The map interacts directly with the player object (combat, movement, etc.)
	Map snailsville("The Snailsville", &player);
	Map rustheap("The Rustheap", &player);
	
	// Generate map structure
	// Parameters:
	// - easy locations vector
	// - medium locations vector
	// - hard locations vector
	// - boss locations vector
	// - number of easy rooms
	// - number of medium rooms
	// - number of hard rooms
	snailsville.generateMap(
		easy_snailsville_locations,
		medium_snailsville_locations,
		hard_snailsville_locations,
		boss_snailsville_locations,
		5, 4, 3
		);
	
	// Store maps in a vector (allows future expansion to multiple maps)
	vector<Map*> maps = { &snailsville };
	
	// Fast-forward map to the saved room (for new games = 0, for loads = saved position)
	snailsville.setPlayerPosition(currentMapIndex);
	
	// =============================================
	//  MAIN GAME LOOP
	// =============================================
	while (true) {
		Map* currentMap = maps[0];
		
		// Save before every room — position is the current room index
		{
			string label = currentMap->getName() + " - " + currentMap->getLocations()[currentMap->getPlayerPosition()]->getName();
			saveGame(currentMap->getPlayerPosition(), player, slot, label);
		}
		
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << player;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		
		currentMap->movePlayer();
		
		// ---- DEATH HANDLING ----
		if (player.getCurrentHp() <= 0) {
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "   The world goes dark around you..." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			
			if (player.useSoulStone()) {
				// Reload the save — which was written right before this room
				int savedRoom;
				loadGame(savedRoom, player, slot);
				currentMap->setPlayerPosition(savedRoom);
				
				// *** CRITICAL FIX: SAVE AFTER USING SOUL STONE ***
				{
					string label = currentMap->getName() + " - " + currentMap->getLocations()[savedRoom]->getName();
					saveGame(savedRoom, player, slot, label);
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
				// No stones — permanent death
				cout << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				cout << " -> No Soul Stones remain." << endl;
				cout << " -> The darkness claims you completely." << endl;
				cout << " -> " << player.getName() << "'s journey ends here." << endl;
				cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				deleteSave(slot);
				system("pause");
				break;
			}
		}
		
		// ---- MAP COMPLETION ----
		if (currentMap->getPlayerFinished()) {
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << " You have survived." << endl;
			cout << " " << player.getName() << " emerges from the dark." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			deleteSave(slot);
			system("pause");
			break;
		}
	}
	
	return 0;
}
