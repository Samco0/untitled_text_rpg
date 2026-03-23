#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <filesystem>
#include <windows.h>
#include "battleManager.h"
#include "chainingspell.h"
#include "statuseffectspell.h"
#include "lifestealspell.h"
#include "map.h"
#include "weapon.h"
#include "savesystem.h"

using namespace std;
namespace fs = std::filesystem;

// =============================================
//  TERMINAL COLOR HELPERS
// =============================================
static void setMapColor(const string& mapName) {
	if (mapName == "The Snailsville")
		cout << "\033[32m";
	else if (mapName == "The Rustheap")
		cout << "\033[33m";
}

static void resetColor() {
	cout << "\033[90m";
}

int main() {
	srand(time(NULL));
	SetConsoleTitle("Untitled Text RPG by Samqo");
	
	int slot = 1;
	int currentMapIndex = 0;  // room within map
	int currentMapIdx = 0;    // which map
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
			if (loadGame(currentMapIndex, currentMapIdx, player, slot)){
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
	saveGame(currentMapIndex, currentMapIdx, player, slot);
	
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
	Scroll easy_snailsville_enemy1_drop2("", &easy_snailsville_enemy1_spell1);
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
	Scroll easy_snailsville_enemy2_drop2("", &easy_snailsville_enemy2_spell1);
	Scroll easy_snailsville_enemy2_drop3("", &easy_snailsville_enemy2_spell2);
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
	Scroll easy_snailsville_enemy3_drop2("", &easy_snailsville_enemy3_spell1);
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
	Scroll easy_snailsville_enemy4_drop3("", &easy_snailsville_enemy4_spell1);
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
	Scroll easy_snailsville_enemy5_drop3("", &easy_snailsville_enemy5_spell2);
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
	Scroll easy_snailsville_enemy6_drop3("", &easy_snailsville_enemy6_spell1);
	Scroll easy_snailsville_enemy6_drop4("", &easy_snailsville_enemy6_spell2);
	Scroll easy_snailsville_enemy6_drop5("", &easy_snailsville_enemy6_spell3);
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
	Scroll easy_snailsville_enemy7_drop2("", &easy_snailsville_enemy7_spell1);
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
	Scroll easy_snailsville_enemy8_drop2("", &easy_snailsville_enemy8_spell1);
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
	Scroll easy_snailsville_enemy9_drop2("", &easy_snailsville_enemy9_spell1);
	Scroll easy_snailsville_enemy9_drop3("", &easy_snailsville_enemy9_spell2);
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
	Scroll easy_treasure1_scroll("", &easy_snailsville_enemy1_spell1);
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
	Scroll easy_treasure2_scroll("", &easy_snailsville_enemy3_spell1);
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
	Scroll easy_treasure3_scroll("", &easy_snailsville_enemy6_spell1);
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
	Scroll medium_snailsville_enemy1_drop6("", &medium_snailsville_enemy1_spell1);
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
	Scroll medium_snailsville_enemy2_drop2("", &medium_snailsville_enemy2_spell1);
	Scroll medium_snailsville_enemy2_drop3("", &medium_snailsville_enemy2_spell2);
	Scroll medium_snailsville_enemy2_drop4("", &medium_snailsville_enemy2_spell3);
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
	Scroll medium_snailsville_enemy3_drop2("", &medium_snailsville_enemy3_spell1);
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
	Scroll medium_snailsville_enemy4_drop6("", &medium_snailsville_enemy4_spell1, "Tome");
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
	Scroll medium_snailsville_enemy5_drop2("", &medium_snailsville_enemy5_spell1, "Tome");
	Scroll medium_snailsville_enemy5_drop3("", &medium_snailsville_enemy5_spell2, "Tome");
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
	Scroll medium_snailsville_enemy7_drop2("", &medium_snailsville_enemy7_spell1);
	Scroll medium_snailsville_enemy7_drop3("", &medium_snailsville_enemy7_spell2);
	Scroll medium_snailsville_enemy7_drop4("", &medium_snailsville_enemy7_spell3);
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
	Scroll medium_treasure1_scroll("", &medium_snailsville_enemy1_spell1);
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
	Scroll medium_treasure2_scroll1("", &medium_snailsville_enemy5_spell1, "Tome");
	Scroll medium_treasure2_scroll2("", &medium_snailsville_enemy5_spell2, "Tome");
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
	Scroll hard_snailsville_enemy1_drop1("", &hard_snailsville_enemy1_spell2);
	Scroll hard_snailsville_enemy1_drop2("", &hard_snailsville_enemy1_spell3);
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
	Scroll hard_snailsville_enemy2_drop2("", &hard_snailsville_enemy2_spell1);
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
	Scroll hard_snailsville_enemy3_drop2("", &hard_snailsville_enemy3_spell1);
	Scroll hard_snailsville_enemy3_drop3("", &hard_snailsville_enemy3_spell2);
	Scroll hard_snailsville_enemy3_drop4("", &hard_snailsville_enemy3_spell3);
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
	Scroll hard_snailsville_enemy4_drop5("", &hard_snailsville_enemy4_spell1);
	Scroll hard_snailsville_enemy4_drop6("", &hard_snailsville_enemy4_spell2);
	Scroll hard_snailsville_enemy4_drop7("", &hard_snailsville_enemy4_spell3);
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
	Scroll hard_snailsville_enemy5_drop3("", &hard_snailsville_enemy5_spell1);
	Scroll hard_snailsville_enemy5_drop4("", &hard_snailsville_enemy5_spell2);
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
	Scroll hard_snailsville_enemy6_drop4("", &hard_snailsville_enemy6_spell1);
	Scroll hard_snailsville_enemy6_drop5("", &hard_snailsville_enemy6_spell2);
	Scroll hard_snailsville_enemy6_drop6("", &hard_snailsville_enemy6_spell3);
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
	Scroll hard_treasure1_scroll1("", &hard_snailsville_enemy1_spell2);
	Scroll hard_treasure1_scroll2("", &hard_snailsville_enemy1_spell3);
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
	Scroll hard_treasure2_scroll1("", &hard_snailsville_enemy5_spell1);
	Scroll hard_treasure2_scroll2("", &hard_snailsville_enemy5_spell2);
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
	Scroll snailsville_boss1_drop3("", &snailsville_boss1_spell1);
	Scroll snailsville_boss1_drop4("", &snailsville_boss1_spell2);
	Scroll snailsville_boss1_drop5("", &snailsville_boss1_spell3);
	Scroll snailsville_boss1_drop6("", &snailsville_boss1_spell4);
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
	HpStatusEffect snailsville_boss2_status1("Hemorrhaging", 6, 3, -2.5);
	StatusEffectSpell snailsville_boss2_spell4("Bloody Web", "The user conjures a crimson web of congealed blood, slicing through the target.", 5, 3, 70, &snailsville_boss2_status1, "Blood");
	Spell* snailsville_boss2_spells[4] = {&snailsville_boss2_spell1, &snailsville_boss2_spell2, &snailsville_boss2_spell3, &snailsville_boss2_spell4};
	Enemy snailsville_boss2("Mathian, the Bloodcarapace", 90, 7, 7, 5, 12, 1.5, snailsville_boss2_spells);
	Armor snailsville_boss2_drop1("Mathian's Carapace of Hemogore", "A chestplate molded from the hardened blood of countless victims, pulsating with dark vitality.", 8, 2);
	Armor snailsville_boss2_drop2("Mathian's Veinbound Greaves", "Leggings crafted from interlaced veins and coagulated blood, unnervingly alive.", 5, 4);
	Weapon snailsville_boss2_drop3("Mathian's Crimson Reaver", "Sword", "A blood-forged blade saturated with the essence of the slain.", 6);
	Scroll snailsville_boss2_drop4("", &snailsville_boss2_spell1);
	Scroll snailsville_boss2_drop5("", &snailsville_boss2_spell2);
	Scroll snailsville_boss2_drop6("", &snailsville_boss2_spell3);
	Scroll snailsville_boss2_drop7("", &snailsville_boss2_spell4);
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
	HpStatusEffect snailsville_boss3_status1("Drowning", 5, 1, -4);
	StatusEffectSpell snailsville_boss3_spell3("Choking Bubble", "The user traps the target in a suffocating bubble", 5, 3, 65, &snailsville_boss3_status1, "Water");
	Spell snailsville_boss3_spell4("Maelstrom Spin", "The user creates a violent whirlpool that drags the target in", 9, 3, "Water");
	Spell* snailsville_boss3_spells[4] = {&snailsville_boss3_spell1, &snailsville_boss3_spell2, &snailsville_boss3_spell3, &snailsville_boss3_spell4};
	Enemy snailsville_boss3("Andreas, the Tidal Snail", 100, 7, 7, 3, 12, 1.5, snailsville_boss3_spells);
	Armor snailsville_boss3_drop1("Andreas's Carapace of Sunken Bones", "Chestplate forged from the bones of creatures lost to the abyss", 8.5, 2);
	Weapon snailsville_boss3_drop2("Andreas's Spear of Drowned Monarchs", "Spear", "A long, coral-encrusted spear once wielded by kings swallowed by the deep", 6);
	Scroll snailsville_boss3_drop3("", &snailsville_boss3_spell1);
	Scroll snailsville_boss3_drop4("", &snailsville_boss3_spell2);
	Scroll snailsville_boss3_drop5("", &snailsville_boss3_spell3);
	Scroll snailsville_boss3_drop6("", &snailsville_boss3_spell4);
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
	HpStatusEffect snailsville_boss4_status1("Cursed Flames", 4, 1, -4);
	StatusEffectSpell snailsville_boss4_spell2("Infernal Breath", "The user exhales a scorching inferno upon the target", 7, 4, 60, &snailsville_boss4_status1, "Fire");
	HpStatusEffect snailsville_boss4_status2("Boiling Veins", 3, 1, -6);
	StatusEffectSpell snailsville_boss4_spell3("Hellfire Heat", "The user burns the air around the target with demonic fire", 5, 5, 40, &snailsville_boss4_status2, "Fire");
	ChainingSpell snailsville_boss4_spell4("Dance of Damnation", "The user writhes possessed by dark flames, striking with ferocity", 2, 3, 2, 5, "Fire");
	Spell* snailsville_boss4_spells[4] = {&snailsville_boss4_spell1, &snailsville_boss4_spell2, &snailsville_boss4_spell3, &snailsville_boss4_spell4};
	Enemy snailsville_boss4("Peklorex, the Infernal Slug Tyrant", 110, 8, 8, 3, 12, 1.6, snailsville_boss4_spells);
	Armor snailsville_boss4_drop1("Peklorex's Obsidian Carapace", "A chestplate forged from cursed snail obsidian, worn by the tyrant", 10, 2);
	Armor snailsville_boss4_drop2("Peklorex's Gauntlets of Torment", "Gauntlets infused with the essence of fallen victims", 3.5, 3);
	Weapon snailsville_boss4_drop3("Peklorex's Soulfire War Axe", "War Axe", "A war axe of snailsidian, carrying the souls of the condemned", 6.5);
	Scroll snailsville_boss4_drop4("", &snailsville_boss4_spell1);
	Scroll snailsville_boss4_drop5("", &snailsville_boss4_spell2);
	Scroll snailsville_boss4_drop6("", &snailsville_boss4_spell3);
	Scroll snailsville_boss4_drop7("", &snailsville_boss4_spell4);
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
	
	// Easy enemies are a little stronger or as strong as the hard enemies in Snailsville, but not stronger.
	// Medium enemies are as strong as bosses in Snailsville.
	// Hard enemies are harder than bosses in Snailsville.
	// Bosses are strong.
	
	// Soulstone drops follow Snailsville precedent: chances scale with difficulty group and individual enemy strength.
	
	// Balance notes (Easy tier target):
	//   HP: 30–50 | Dmg: 4–7 | Level: 5 | Speed: 2–9
	//   Spell dmg per hit: 6–14 | CritChance: 10–20 | CritValue: 1.25–1.5
	//   Drops: every enemy has at least 2 drops; weapon drops are more common than in Snailsville.
	
	//Rustheap | Easy Enemy no. 1 | Gritclaw the Hollow
	// Slow brawler – goes last, trades attrition. Two metal spells, modest spike potential.
	Spell easy_rustheap_enemy1_spell1("Rust-Torn Scrap Bolt", "The user conjures a jagged fragment of rust-eaten metal and hurls it violently toward the target.", 8, 3, "Metal");
	Spell easy_rustheap_enemy1_spell2("Corrosion Talon Lunge", "Twisted scrap talons form around the user's claws as they leap forward in a savage tearing strike.", 7, 2, "Metal");
	Spell* easy_rustheap_enemy1_spells[4] = {&easy_rustheap_enemy1_spell1, &easy_rustheap_enemy1_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy1("Gritclaw, the Hollow", 46, 5.5, 5, 3, 14, 1.4, easy_rustheap_enemy1_spells);
	Consumable easy_rustheap_enemy1_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Scroll easy_rustheap_enemy1_drop2("", &easy_rustheap_enemy1_spell1);
	Weapon easy_rustheap_enemy1_drop3("Scrapfang Talons", "Claws", "Crude claws forged from bent and rusted metal shards.", 5);
	easy_rustheap_enemy1.addReward(&easy_rustheap_enemy1_drop1, 60, 3);
	easy_rustheap_enemy1.addReward(&easy_rustheap_enemy1_drop2, 25, 1);
	easy_rustheap_enemy1.addReward(&easy_rustheap_enemy1_drop3, 15, 1);
	easy_rustheap_enemy1.setSoulStoneDropChance(5);
	easy_rustheap_enemy1.setDescription("A malnourished young raccoon, barely surviving on the lower heaps. Throws corroded junk with trembling claws — desperate and feral, not yet broken enough to stop fighting.");
	CombatLocation easy_rustheap_clocation1("Rustheap Foothills, Scrap Burrow", &easy_rustheap_enemy1);
	vector<Location*> easy_rustheap_locations = {&easy_rustheap_clocation1};
	
	//Rustheap | Easy Enemy no. 2 | Skabscuttle
	// Tank with one slow spell. Low speed means the player should outdamage it with burst.
	Spell easy_rustheap_enemy2_spell1("Heapfall", "The user drags a mound of rotting garbage and collapses it onto the target.", 9, 4, "Garbage");
	Spell* easy_rustheap_enemy2_spells[4] = {&easy_rustheap_enemy2_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy2("Skabscuttle", 58, 6.5, 5, 2, 15, 1.3, easy_rustheap_enemy2_spells);
	Consumable easy_rustheap_enemy2_drop1("Heaproach Leg", "A severed leg from one of the massive cockroaches that infest the lower rustheaps.", 12.5);
	Scroll easy_rustheap_enemy2_drop2("", &easy_rustheap_enemy2_spell1);
	Armor easy_rustheap_enemy2_drop3("Heaproach Carapace", "Armor carved from the hardened shell of a giant heap cockroach.", 7, 2);
	Armor easy_rustheap_enemy2_drop4("Heaproach Skull Helm", "A grotesque helmet fashioned from the hollow head of a giant cockroach.", 4, 1);
	easy_rustheap_enemy2.addReward(&easy_rustheap_enemy2_drop1, 55, 3);
	easy_rustheap_enemy2.addReward(&easy_rustheap_enemy2_drop2, 20, 1);
	easy_rustheap_enemy2.addReward(&easy_rustheap_enemy2_drop3, 20, 1);
	easy_rustheap_enemy2.addReward(&easy_rustheap_enemy2_drop4, 10, 1);
	easy_rustheap_enemy2.setSoulStoneDropChance(5);
	easy_rustheap_enemy2.setDescription("A bloated, grime-slick cockroach that infests the heap's underbelly. Shoves rotting debris into paths, more pest than predator \x97 but pests outlive everything.");
	CombatLocation easy_rustheap_clocation2("Rustheap Foothills, Garbage Chokepoint", &easy_rustheap_enemy2);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation2);
	
	//Rustheap | Easy Enemy no. 3 | Dregfist
	// Two-spell fighter; Pulverized Brickstorm toned down so it doesn't one-combo.
	Spell easy_rustheap_enemy3_spell1("Livewire Knuckle", "The user coils frayed live wires around their fists, crackling with stolen current before striking.", 8, 2, "Electric");
	ChainingSpell easy_rustheap_enemy3_spell2("Pulverized Brickstorm", "The user crushes a brittle brick into choking dust and flings the fragments in a violent scatter.", 0.8, 3, 5, 10, "Physical");
	Spell* easy_rustheap_enemy3_spells[4] = {&easy_rustheap_enemy3_spell1, &easy_rustheap_enemy3_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy3("Dregfist", 48, 6, 5, 4, 16, 1.3, easy_rustheap_enemy3_spells);
	Consumable easy_rustheap_enemy3_drop1("Heaproach Leg", "A severed leg from one of the massive cockroaches that infest the lower rustheaps.", 12.5);
	Scroll easy_rustheap_enemy3_drop2("", &easy_rustheap_enemy3_spell1);
	Weapon easy_rustheap_enemy3_drop3("Crumbling Mason Brick", "Brick", "A cracked brick pried from the collapsing heaps \x97 crude, heavy, and still deadly.", 4.5);
	easy_rustheap_enemy3.addReward(&easy_rustheap_enemy3_drop1, 55, 3);
	easy_rustheap_enemy3.addReward(&easy_rustheap_enemy3_drop2, 25, 1);
	easy_rustheap_enemy3.addReward(&easy_rustheap_enemy3_drop3, 20, 1);
	easy_rustheap_enemy3.setSoulStoneDropChance(6);
	easy_rustheap_enemy3.setDescription("A low-rung rat who swings crumbling bricks with knuckles wrapped in wire. Part of Rustheap's scavenging underclass \x97 more scarred than skilled, but hungrier than most.");
	CombatLocation easy_rustheap_clocation3("Rustheap Foothills, Broken Masonry Runoff", &easy_rustheap_enemy3);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation3);
	
	//Rustheap | Easy Enemy no. 4 | Ashfinger
	// Spell array was nullptr-only (bug). Fixed: spells assigned correctly. Status effects trimmed.
	HpStatusEffect easy_rustheap_enemy4_status1("Suffocating Smoke", 4, 3, 2);
	StatusEffectSpell easy_rustheap_enemy4_spell1("Smog Eruption", "The user violently exhales a blast of choking black smoke into the target's face.", 8, 3, 70, &easy_rustheap_enemy4_status1, "Smoke");
	Spell easy_rustheap_enemy4_spell2("Cinder Knuckle", "The user shapes a dense fist of smoldering ash and slams it into the target.", 9, 3, "Ash");
	HpStatusEffect easy_rustheap_enemy4_status2("Scorching Flames", 4, 2, 2.5);
	StatusEffectSpell easy_rustheap_enemy4_spell3("Ember Arrow", "A narrow streak of burning embers is launched toward the target, igniting flesh on impact.", 10, 4, 65, &easy_rustheap_enemy4_status2, "Fire");
	Spell* easy_rustheap_enemy4_spells[4] = {&easy_rustheap_enemy4_spell1, &easy_rustheap_enemy4_spell2, &easy_rustheap_enemy4_spell3, nullptr};
	Enemy easy_rustheap_enemy4("Ashfinger", 46, 6, 5, 5, 20, 1.45, easy_rustheap_enemy4_spells);
	Scroll easy_rustheap_enemy4_drop1("", &easy_rustheap_enemy4_spell2);
	Scroll easy_rustheap_enemy4_drop2("", &easy_rustheap_enemy4_spell3);
	Weapon easy_rustheap_enemy4_drop3("Cinderforged Blade", "Sword", "A brittle blade formed from compressed ash and soot.", 4.5);
	easy_rustheap_enemy4.addReward(&easy_rustheap_enemy4_drop1, 25, 1);
	easy_rustheap_enemy4.addReward(&easy_rustheap_enemy4_drop2, 25, 1);
	easy_rustheap_enemy4.addReward(&easy_rustheap_enemy4_drop3, 20, 1);
	easy_rustheap_enemy4.setSoulStoneDropChance(4);
	easy_rustheap_enemy4.setDescription("A twitchy, burn-scarred creature addicted to the smell of smoke. Ignites small fires out of compulsion \x97 some say they no longer feel pain from flame. Their fingertips are permanently charred black.");
	CombatLocation easy_rustheap_clocation4("Rustheap Foothills, Smoldering Waste Pit", &easy_rustheap_enemy4);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation4);
	
	//Rustheap | Easy Enemy no. 5 | Rattlegrip
	// Three-spell generalist; drop table expanded with consumable for more variety.
	Spell easy_rustheap_enemy5_spell1("Scrapshot Discharge", "The user crudely shapes a shard of metal into a makeshift projectile and fires it toward the target.", 8, 3, "Metal");
	Spell easy_rustheap_enemy5_spell2("Forged Iron Blow", "The user coats their fist in hardened scrap metal before driving it into the target.", 10, 4, "Metal");
	Spell easy_rustheap_enemy5_spell3("Crackling Surge", "A burst of unstable electricity erupts from the user's palm, jolting the target violently.", 7, 2, "Electric");
	Spell* easy_rustheap_enemy5_spells[4] = {&easy_rustheap_enemy5_spell1, &easy_rustheap_enemy5_spell2, &easy_rustheap_enemy5_spell3, nullptr};
	Enemy easy_rustheap_enemy5("Rattlegrip", 38, 5, 5, 6, 12, 1.35, easy_rustheap_enemy5_spells);
	Consumable easy_rustheap_enemy5_drop1("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10);
	Armor easy_rustheap_enemy5_drop2("Rustbound Helm", "A crude helmet hammered together from rusted scrap plates.", 4, 1);
	Armor easy_rustheap_enemy5_drop3("Patchwork Scrap Cuirass", "A chestplate assembled from mismatched pieces of rusted metal.", 6, 2);
	Weapon easy_rustheap_enemy5_drop4("Rustheap Torque Wrench", "Wrench", "A heavy industrial wrench once used by Rustheap engineers.", 4.5);
	easy_rustheap_enemy5.addReward(&easy_rustheap_enemy5_drop1, 55, 2);
	easy_rustheap_enemy5.addReward(&easy_rustheap_enemy5_drop2, 20, 1);
	easy_rustheap_enemy5.addReward(&easy_rustheap_enemy5_drop3, 20, 1);
	easy_rustheap_enemy5.addReward(&easy_rustheap_enemy5_drop4, 15, 1);
	easy_rustheap_enemy5.setSoulStoneDropChance(4);
	easy_rustheap_enemy5.setDescription("A trembling apprentice barely keeping their contraptions from falling apart. The older engineers beat new techniques into them. Every scar on their hands is a lesson.");
	CombatLocation easy_rustheap_clocation5("Rustheap Foothills, Abandoned Technician Shack", &easy_rustheap_enemy5);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation5);
	
	//Rustheap | Easy Enemy no. 6 | The Visorless
	// Single fast charge. Very small drop table expanded with a consumable.
	Spell easy_rustheap_enemy6_spell1("Speed Drive", "The user launches forward in a brutal, bone-rattling charge, slamming into the target with reckless mechanical force.", 11, 5, "Physical");
	Spell* easy_rustheap_enemy6_spells[4] = {&easy_rustheap_enemy6_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy6("The Visorless", 40, 5.5, 5, 6, 15, 1.25, easy_rustheap_enemy6_spells);
	Consumable easy_rustheap_enemy6_drop1("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10);
	Scroll easy_rustheap_enemy6_drop2("", &easy_rustheap_enemy6_spell1);
	Armor easy_rustheap_enemy6_drop3("Fractured Rust Visor", "A cracked protective visor ripped from a scavenger driver somewhere in the lower Rustheap trails.", 4.5, 1);
	easy_rustheap_enemy6.addReward(&easy_rustheap_enemy6_drop1, 55, 2);
	easy_rustheap_enemy6.addReward(&easy_rustheap_enemy6_drop2, 30, 1);
	easy_rustheap_enemy6.addReward(&easy_rustheap_enemy6_drop3, 20, 1);
	easy_rustheap_enemy6.setSoulStoneDropChance(6);
	easy_rustheap_enemy6.setDescription("A skeletal one-seat vehicle stitched from wreck parts and spite. The driver's face is never seen beneath the cracked visor \x97 only teeth, when they charge.");
	CombatLocation easy_rustheap_clocation6("Rustheap Foothills, Wreckslide Path", &easy_rustheap_enemy6);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation6);
	
	//Rustheap | Easy Enemy no. 7 | Hookwire
	// Armor-break debuffer. Dmg tuned down; second drop added (consumable).
	Spell easy_rustheap_enemy7_spell1("Rustflail Tail", "The creature lashes its wire-wrapped tail through the air, striking the target with a savage metallic crack.", 9, 3, "Physical");
	ArmorBreakStatusEffect easy_rustheap_enemy7_status1("Shackled Armor", 1, 4);
	StatusEffectSpell easy_rustheap_enemy7_spell2("Chain of Despair", "A jagged spectral chain coils around the target, crushing joints in their armor and weakening its protection.", 7, 3, 70, &easy_rustheap_enemy7_status1, "Chains");
	Spell* easy_rustheap_enemy7_spells[4] = {&easy_rustheap_enemy7_spell1, &easy_rustheap_enemy7_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy7("Hookwire", 50, 6.5, 5, 5, 20, 1.2, easy_rustheap_enemy7_spells);
	Consumable easy_rustheap_enemy7_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Weapon easy_rustheap_enemy7_drop2("Barbed Wire Tail", "Club", "A severed raccoon tail stiffened with rusted wire and hooked scrap.", 5);
	easy_rustheap_enemy7.addReward(&easy_rustheap_enemy7_drop1, 55, 3);
	easy_rustheap_enemy7.addReward(&easy_rustheap_enemy7_drop2, 25, 1);
	easy_rustheap_enemy7.setSoulStoneDropChance(5);
	easy_rustheap_enemy7.setDescription("A young raccoon engineer still learning the heap's dark craft. The tail is wrapped in wire and jagged scrap \x97 not a weapon by design, but by necessity.");
	CombatLocation easy_rustheap_clocation7("Rustheap Foothills, Tangled Wire Ravine", &easy_rustheap_enemy7);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation7);
	
	//Rustheap | Easy Enemy no. 8 | Splicemite
	// Rot-DOT specialist. Status values toned down; weapon drop added.
	HpStatusEffect easy_rustheap_enemy8_status1("Seeping Rot", 4, 1, -3);
	StatusEffectSpell easy_rustheap_enemy8_spell1("Rotbreath Miasma", "A foul cloud of rot spores leaks from the creature's broken teeth, drifting into the lungs of its target.", 8, 3, 80, &easy_rustheap_enemy8_status1, "Rot");
	ArmorBreakStatusEffect easy_rustheap_enemy8_status2("Corroded Plating", 1, 3);
	StatusEffectSpell easy_rustheap_enemy8_spell2("Corrosive Spittle", "The creature hacks up a string of infected rot and spits it onto the target's armor, slowly eating through the material.", 7, 3, 45, &easy_rustheap_enemy8_status2, "Rot");
	Spell* easy_rustheap_enemy8_spells[4] = {&easy_rustheap_enemy8_spell1, &easy_rustheap_enemy8_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy8("Splicemite", 34, 4.5, 5, 7, 15, 1.35, easy_rustheap_enemy8_spells);
	Consumable easy_rustheap_enemy8_drop1("Rotroach Leg", "A severed limb from the bloated rustheap cockroaches that crawl through the lower scrap pits.", 12.5);
	Scroll easy_rustheap_enemy8_drop2("", &easy_rustheap_enemy8_spell1);
	Weapon easy_rustheap_enemy8_drop3("Infected Spliceblade", "Knife", "A short blade salvaged from a failed augment surgery, still weeping greenish fluid.", 4);
	easy_rustheap_enemy8.addReward(&easy_rustheap_enemy8_drop1, 55, 3);
	easy_rustheap_enemy8.addReward(&easy_rustheap_enemy8_drop2, 25, 1);
	easy_rustheap_enemy8.addReward(&easy_rustheap_enemy8_drop3, 15, 1);
	easy_rustheap_enemy8.setSoulStoneDropChance(6);
	easy_rustheap_enemy8.setDescription("A cockroach whose flesh has been crudely fused with salvaged machine parts \x97 not by choice. The augments are infected, weeping fluid. It bites to spread the rot.");
	CombatLocation easy_rustheap_clocation8("Rustheap Foothills, Rotting Augment Pit", &easy_rustheap_enemy8);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation8);
	
	//Rustheap | Easy Enemy no. 9 | Rustvow
	// Fast volley-caster; chaining spells tuned so burst is scary but not lethal.
	ChainingSpell easy_rustheap_enemy9_spell1("Shattering Glassstorm", "The user summons a storm of jagged glass splinters and flings them toward the target in rapid succession.", 0.8, 3, 7, 12, "Glass");
	ChainingSpell easy_rustheap_enemy9_spell2("Rusted Bolt Volley", "A cluster of crude metal bolts materializes and is hurled violently toward the target.", 2.5, 4, 3, 5, "Metal");
	Spell* easy_rustheap_enemy9_spells[4] = {&easy_rustheap_enemy9_spell1, &easy_rustheap_enemy9_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy9("Rustvow", 32, 4, 5, 8, 18, 1.5, easy_rustheap_enemy9_spells);
	Consumable easy_rustheap_enemy9_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Scroll easy_rustheap_enemy9_drop2("", &easy_rustheap_enemy9_spell1);
	Scroll easy_rustheap_enemy9_drop3("", &easy_rustheap_enemy9_spell2);
	easy_rustheap_enemy9.addReward(&easy_rustheap_enemy9_drop1, 55, 3);
	easy_rustheap_enemy9.addReward(&easy_rustheap_enemy9_drop2, 25, 1);
	easy_rustheap_enemy9.addReward(&easy_rustheap_enemy9_drop3, 20, 1);
	easy_rustheap_enemy9.setSoulStoneDropChance(5);
	easy_rustheap_enemy9.setDescription("A true believer in Rustheap's creed \x97 society is a cage and order is a disease. Throws anything within reach: bolts, teeth, bones, glass.");
	CombatLocation easy_rustheap_clocation9("Rustheap Foothills, Shardfall Gutter", &easy_rustheap_enemy9);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation9);
	
	//Rustheap | Easy Enemy no. 10 | Gutborer
	// Ambush brawler. Single spike spell; drop table was bare, now has consumable + armor.
	Spell easy_rustheap_enemy10_spell1("Ground and Pound", "The creature burrows beneath the scrap-choked earth before erupting upward with a brutal uppercut.", 13, 5, "Physical");
	Spell* easy_rustheap_enemy10_spells[4] = {&easy_rustheap_enemy10_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy10("Gutborer", 55, 7, 5, 3, 14, 1.45, easy_rustheap_enemy10_spells);
	Consumable easy_rustheap_enemy10_drop1("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5);
	Scroll easy_rustheap_enemy10_drop2("", &easy_rustheap_enemy10_spell1);
	Armor easy_rustheap_enemy10_drop3("Scrap-Plated Gut Guard", "A curved plate of layered metal covering the midsection, salvaged from wrecked machinery.", 5.5, 2);
	easy_rustheap_enemy10.addReward(&easy_rustheap_enemy10_drop1, 55, 2);
	easy_rustheap_enemy10.addReward(&easy_rustheap_enemy10_drop2, 25, 1);
	easy_rustheap_enemy10.addReward(&easy_rustheap_enemy10_drop3, 20, 1);
	easy_rustheap_enemy10.setSoulStoneDropChance(7);
	easy_rustheap_enemy10.setDescription("A worm-shaped construct of unknown origin, writhing beneath the heap's surface. It erupts from below without warning, dragging things into the dark.");
	CombatLocation easy_rustheap_clocation10("Rustheap Foothills, Burrowed Scrapbed", &easy_rustheap_enemy10);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation10);
	
	//Rustheap | Easy Enemy no. 11 | Shardback
	// Armor-breaker; weapon drop added. Spells tuned to fair levels.
	Spell easy_rustheap_enemy11_spell1("Spinebreaker Slam", "The creature twists its shard-covered back and crashes into the target with crushing metallic force.", 11, 3, "Metal");
	ArmorBreakStatusEffect easy_rustheap_enemy11_status1("Shattered Guard", 1, 4);
	StatusEffectSpell easy_rustheap_enemy11_spell2("Razor Tail Flail", "Its jagged tail lashes out wildly, tearing into armor and sending fragments flying.", 9, 3, 70, &easy_rustheap_enemy11_status1, "Physical");
	Spell* easy_rustheap_enemy11_spells[4] = {&easy_rustheap_enemy11_spell1, &easy_rustheap_enemy11_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy11("Shardback", 36, 5, 5, 7, 12, 1.5, easy_rustheap_enemy11_spells);
	Consumable easy_rustheap_enemy11_drop1("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15);
	Scroll easy_rustheap_enemy11_drop2("", &easy_rustheap_enemy11_spell1);
	Weapon easy_rustheap_enemy11_drop3("Shard-Edged Tailblade", "Sword", "A curved blade filed from a gecko's detached shard-tail. The edge is uneven but punishing.", 5);
	easy_rustheap_enemy11.addReward(&easy_rustheap_enemy11_drop1, 55, 3);
	easy_rustheap_enemy11.addReward(&easy_rustheap_enemy11_drop2, 25, 1);
	easy_rustheap_enemy11.addReward(&easy_rustheap_enemy11_drop3, 15, 1);
	easy_rustheap_enemy11.setSoulStoneDropChance(5);
	easy_rustheap_enemy11.setDescription("A gecko whose back has fused with metal shards over years of living in the heap. The tail swings not by training, but by rage \x97 a body becoming one with the refuse.");
	CombatLocation easy_rustheap_clocation11("Rustheap Foothills, Shardscale Nest", &easy_rustheap_enemy11);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation11);
	
	//Rustheap | Easy Enemy no. 12 | Twitch-Copper
	// Electric multi-spell; chaining surge balanced to not overwhelm. Weapon drop kept.
	Spell easy_rustheap_enemy12_spell1("Static Discharge", "The creature drags its copper-threaded body across scrap, building a violent charge before releasing it in a blinding arc of electricity.", 12, 5, "Electricity");
	ChainingSpell easy_rustheap_enemy12_spell2("Twitching Surge", "Its body convulses uncontrollably as it flickers in and out of sight, striking the target in erratic bursts of crackling energy.", 2, 4, 3, 5, "Electricity");
	Spell* easy_rustheap_enemy12_spells[4] = {&easy_rustheap_enemy12_spell1, &easy_rustheap_enemy12_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy12("Dawe, the Twitch-Copper", 34, 5, 5, 6, 12, 1.4, easy_rustheap_enemy12_spells);
	Consumable easy_rustheap_enemy12_drop1("Wire-Fed Rat Flesh", "Stringy meat laced with thin copper filaments that still faintly hum with residual charge.", 9.5);
	Scroll easy_rustheap_enemy12_drop2("", &easy_rustheap_enemy12_spell1);
	Weapon easy_rustheap_enemy12_drop3("Livewire Spine", "Livewire Spine", "A spine threaded with copper veins that twitch with leftover current, as if remembering something it shouldn't.", 5.5);
	easy_rustheap_enemy12.addReward(&easy_rustheap_enemy12_drop1, 55, 3);
	easy_rustheap_enemy12.addReward(&easy_rustheap_enemy12_drop2, 25, 1);
	easy_rustheap_enemy12.addReward(&easy_rustheap_enemy12_drop3, 15, 1);
	easy_rustheap_enemy12.setSoulStoneDropChance(4);
	easy_rustheap_enemy12.setDescription("A rat whose nervous system has been threaded with exposed copper wire. Every twitch sends sparks flying. Some believe they no longer sleep \x97 just convulse. It mutters about taking someone far beyond the sky, somewhere unreachable.");
	CombatLocation easy_rustheap_clocation12("Rustheap Foothills, Flickercoil Den", &easy_rustheap_enemy12);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation12);
	
	//Rustheap | Easy Enemy no. 13 | Hollowbeat
	// Sound/armor-break hybrid; weapon drop added to round out loot.
	Spell easy_rustheap_enemy13_spell1("Hollow Resonance", "A deep, unnatural hum escapes the creature, vibrating through bone and scrap alike.", 12, 5, "Sound");
	ArmorBreakStatusEffect easy_rustheap_enemy13_status1("Disrupted Rhythm", 1, 3);
	StatusEffectSpell easy_rustheap_enemy13_spell2("Rhythm Distortion", "The creature emits a warped, broken cadence that throws the target's armor out of sync, weakening its structure.", 9, 3, 70, &easy_rustheap_enemy13_status1, "Sound");
	Spell* easy_rustheap_enemy13_spells[4] = {&easy_rustheap_enemy13_spell1, &easy_rustheap_enemy13_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy13("Hollowbeat", 50, 6, 5, 5, 14, 1.45, easy_rustheap_enemy13_spells);
	Consumable easy_rustheap_enemy13_drop1("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15);
	Scroll easy_rustheap_enemy13_drop2("", &easy_rustheap_enemy13_spell1);
	Weapon easy_rustheap_enemy13_drop3("Resonance Mallet", "Hammer", "A dense hammer whose head hums with a dull vibration. Strikes land strange.", 4.5);
	easy_rustheap_enemy13.addReward(&easy_rustheap_enemy13_drop1, 55, 3);
	easy_rustheap_enemy13.addReward(&easy_rustheap_enemy13_drop2, 25, 1);
	easy_rustheap_enemy13.addReward(&easy_rustheap_enemy13_drop3, 15, 1);
	easy_rustheap_enemy13.setSoulStoneDropChance(5);
	easy_rustheap_enemy13.setDescription("A gecko who found religion in sound. Beats the ground with its tail in hypnotic rhythms that rattle teeth. The thuds are meditative until they aren't.");
	CombatLocation easy_rustheap_clocation13("Rustheap Foothills, Resonance Pit", &easy_rustheap_enemy13);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation13);
	
	//Rustheap | Easy Enemy no. 14 | Lowmarrow
	// Slow internal-bleed caster; DOT tick and duration trimmed. Armor drop kept.
	HpStatusEffect easy_rustheap_enemy14_status1("Internal Hemorrhage", 4, 3, -3);
	StatusEffectSpell easy_rustheap_enemy14_spell1("Concussive Rumble", "A deep, sub-audible vibration erupts from the creature's ribcage, pulsing through the body and rupturing tissue from within.", 12, 5, 70, &easy_rustheap_enemy14_status1, "Sound");
	Spell* easy_rustheap_enemy14_spells[4] = {&easy_rustheap_enemy14_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy14("Lowmarrow", 52, 6.5, 5, 3, 12, 1.45, easy_rustheap_enemy14_spells);
	Consumable easy_rustheap_enemy14_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Armor easy_rustheap_enemy14_drop2("Subsonic Ribcage Plate", "A chestplate formed from a vibrating rib structure that emits a constant, barely audible hum.", 6, 2);
	Scroll easy_rustheap_enemy14_drop3("", &easy_rustheap_enemy14_spell1);
	easy_rustheap_enemy14.addReward(&easy_rustheap_enemy14_drop1, 55, 3);
	easy_rustheap_enemy14.addReward(&easy_rustheap_enemy14_drop2, 20, 1);
	easy_rustheap_enemy14.addReward(&easy_rustheap_enemy14_drop3, 20, 1);
	easy_rustheap_enemy14.setSoulStoneDropChance(4);
	easy_rustheap_enemy14.setDescription("A young raccoon whose ribcage vibrates at sub-audible frequencies. When the sound rises, the air around them shimmers. Internal bleeding is common in those who stand too close.");
	CombatLocation easy_rustheap_clocation14("Rustheap Foothills, Subsonic Hollow", &easy_rustheap_enemy14);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation14);
	
	//Rustheap | Easy Enemy no. 15 | Woundedge
	// Bleed specialist; DOT tightened. Weapon drop chance raised.
	Spell easy_rustheap_enemy15_spell1("Ragged Carve", "The user drags a broken, jagged edge across the target, tearing flesh in an uneven, brutal cut.", 10, 2, "Physical");
	HpStatusEffect easy_rustheap_enemy15_status1("Jagged Bleeding", 1, 4, -2.5);
	StatusEffectSpell easy_rustheap_enemy15_spell2("Serrated Rupture", "A vicious slash from the crude blade leaves a wound that refuses to close, continuing to tear with every movement.", 10, 3, 70, &easy_rustheap_enemy15_status1, "Metal");
	Spell* easy_rustheap_enemy15_spells[4] = {&easy_rustheap_enemy15_spell1, &easy_rustheap_enemy15_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy15("Woundedge", 48, 7, 5, 7, 16, 1.3, easy_rustheap_enemy15_spells);
	Consumable easy_rustheap_enemy15_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Scroll easy_rustheap_enemy15_drop2("", &easy_rustheap_enemy15_spell2);
	Weapon easy_rustheap_enemy15_drop3("Serrated Scrap Blade", "Knife", "A crude blade forged from a shattered augment, its edge uneven and cruel.", 5);
	easy_rustheap_enemy15.addReward(&easy_rustheap_enemy15_drop1, 55, 3);
	easy_rustheap_enemy15.addReward(&easy_rustheap_enemy15_drop2, 25, 1);
	easy_rustheap_enemy15.addReward(&easy_rustheap_enemy15_drop3, 20, 1);
	easy_rustheap_enemy15.setSoulStoneDropChance(5);
	easy_rustheap_enemy15.setDescription("A raccoon who shaped their weapon from their own broken augment after it failed mid-surgery. The blade is ugly and jagged \x97 the anger behind it isn't.");
	CombatLocation easy_rustheap_clocation15("Rustheap Foothills, Bloodscrap Alley", &easy_rustheap_enemy15);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation15);
	
	//Rustheap | Easy Enemy no. 16 | Blastmite
	// Single explosion nuke; fast, fragile. Weapon drop added.
	Spell easy_rustheap_enemy16_spell1("Sudden Ignition", "Without warning, the creature erupts into violent flames, releasing a contained explosion while its own body remains intact.", 13, 5, "Explosion");
	Spell* easy_rustheap_enemy16_spells[4] = {&easy_rustheap_enemy16_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy16("Blastmite", 30, 5.5, 5, 8, 12, 1.4, easy_rustheap_enemy16_spells);
	Consumable easy_rustheap_enemy16_drop1("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15);
	Scroll easy_rustheap_enemy16_drop2("", &easy_rustheap_enemy16_spell1, "Tome");
	Weapon easy_rustheap_enemy16_drop3("Igniton Rod", "Club", "A short metal rod packed with volatile compounds at its tip. Handle with care.", 4);
	easy_rustheap_enemy16.addReward(&easy_rustheap_enemy16_drop1, 55, 3);
	easy_rustheap_enemy16.addReward(&easy_rustheap_enemy16_drop2, 25, 1);
	easy_rustheap_enemy16.addReward(&easy_rustheap_enemy16_drop3, 20, 1);
	easy_rustheap_enemy16.setSoulStoneDropChance(4);
	easy_rustheap_enemy16.setDescription("A small gecko packed with improvised charges in the folds of its skin. Expendable. Replaceable. The heap produces more of them every season.");
	CombatLocation easy_rustheap_clocation16("Rustheap Foothills, Ignition Nest", &easy_rustheap_enemy16);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation16);
	
	//Rustheap | Easy Enemy no. 17 | Bilespew
	// Triple-spell fire boss of the easy tier. Spells scaled back slightly; stays the hardest easy.
	HpStatusEffect easy_rustheap_enemy17_status1("Clinging Bile Burn", 5, 3, -2);
	StatusEffectSpell easy_rustheap_enemy17_spell1("Scalding Bile Spit", "The creature spews a stream of superheated bile that sticks to flesh, burning long after impact.", 10, 4, 75, &easy_rustheap_enemy17_status1, "Fire");
	Spell easy_rustheap_enemy17_spell2("Furnace Breath", "A violent stream of fire erupts from the creature's maw, engulfing everything in front of it.", 12, 5, "Fire");
	HpStatusEffect easy_rustheap_enemy17_status2("Blazing Laceration", 4, 2, -2.5);
	StatusEffectSpell easy_rustheap_enemy17_spell3("Emberclaw Rend", "The creature forms claws of living flame and tears into the target, leaving burning wounds behind.", 11, 4, 75, &easy_rustheap_enemy17_status2, "Fire");
	Spell* easy_rustheap_enemy17_spells[4] = {&easy_rustheap_enemy17_spell1, &easy_rustheap_enemy17_spell2, &easy_rustheap_enemy17_spell3, nullptr};
	Enemy easy_rustheap_enemy17("Bilespew", 58, 7, 5, 5, 14, 1.35, easy_rustheap_enemy17_spells);
	Consumable easy_rustheap_enemy17_drop1("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20);
	Scroll easy_rustheap_enemy17_drop2("", &easy_rustheap_enemy17_spell1);
	Scroll easy_rustheap_enemy17_drop3("", &easy_rustheap_enemy17_spell2);
	Weapon easy_rustheap_enemy17_drop4("Emberforged Blade", "Sword", "A blade formed from condensed flame, flickering as if barely contained.", 5.5);
	easy_rustheap_enemy17.addReward(&easy_rustheap_enemy17_drop1, 50, 1);
	easy_rustheap_enemy17.addReward(&easy_rustheap_enemy17_drop2, 25, 1);
	easy_rustheap_enemy17.addReward(&easy_rustheap_enemy17_drop3, 20, 1);
	easy_rustheap_enemy17.addReward(&easy_rustheap_enemy17_drop4, 20, 1);
	easy_rustheap_enemy17.setSoulStoneDropChance(7);
	easy_rustheap_enemy17.setDescription("Born near the heap's furnace pits, their saliva runs hot. Not engineered \x97 adapted. They spit superheated bile that clings to flesh.");
	CombatLocation easy_rustheap_clocation17("Rustheap Foothills, Furnace Runoff", &easy_rustheap_enemy17);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation17);
	
	//Rustheap | Easy Enemy no. 18 | Ashseat
	// Fast ash-driver. Was drop-starved (only a scroll). Now has consumable + armor too.
	Spell easy_rustheap_enemy18_spell1("Ash Drive", "The user ignites a burst of ash beneath them, propelling forward in a violent lunge that leaves a choking trail behind.", 12, 5, "Ash");
	Spell* easy_rustheap_enemy18_spells[4] = {&easy_rustheap_enemy18_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy18("Ashseat", 38, 5.5, 5, 7, 12, 1.2, easy_rustheap_enemy18_spells);
	Consumable easy_rustheap_enemy18_drop1("Cracked Fuel Canteen", "A dented canteen of heap-refined fuel. Bitter. Burns going down. Keeps you awake.", 10);
	Scroll easy_rustheap_enemy18_drop2("", &easy_rustheap_enemy18_spell1);
	Armor easy_rustheap_enemy18_drop3("Ash-Dusted Driver Coat", "A long coat stiffened with ash and dried blood. It smells like the inside of an engine.", 5, 2);
	easy_rustheap_enemy18.addReward(&easy_rustheap_enemy18_drop1, 55, 2);
	easy_rustheap_enemy18.addReward(&easy_rustheap_enemy18_drop2, 25, 1);
	easy_rustheap_enemy18.addReward(&easy_rustheap_enemy18_drop3, 20, 1);
	easy_rustheap_enemy18.setSoulStoneDropChance(5);
	easy_rustheap_enemy18.setDescription("A young driver who inherited their vehicle from a dead sibling. Too small for the seat, but too proud to admit it. The vehicle smells of ash and blood.");
	CombatLocation easy_rustheap_clocation18("Rustheap Foothills, Ashtrail Descent", &easy_rustheap_enemy18);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation18);
	
	//Rustheap | Easy Enemy no. 19 | Jerkspring
	// Mechanical twitcher; chaining spell tightened. Consumable drop added.
	Spell easy_rustheap_enemy19_spell1("Springloaded Kick", "The creature compresses spring-like limbs and releases them in a sudden, snapping kick aimed with unnatural precision.", 13, 6, "Metal");
	ChainingSpell easy_rustheap_enemy19_spell2("Tick-Twitch Rush", "Its body spasms in rapid mechanical bursts, bouncing erratically as it repeatedly lunges into the target.", 2.5, 4, 2, 4);
	Spell* easy_rustheap_enemy19_spells[4] = {&easy_rustheap_enemy19_spell1, &easy_rustheap_enemy19_spell2, nullptr, nullptr};
	Enemy easy_rustheap_enemy19("Jerkspring", 34, 5, 5, 8, 10, 1.5, easy_rustheap_enemy19_spells);
	Consumable easy_rustheap_enemy19_drop1("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15);
	Scroll easy_rustheap_enemy19_drop2("", &easy_rustheap_enemy19_spell1);
	Scroll easy_rustheap_enemy19_drop3("", &easy_rustheap_enemy19_spell2);
	easy_rustheap_enemy19.addReward(&easy_rustheap_enemy19_drop1, 55, 3);
	easy_rustheap_enemy19.addReward(&easy_rustheap_enemy19_drop2, 25, 1);
	easy_rustheap_enemy19.addReward(&easy_rustheap_enemy19_drop3, 20, 1);
	easy_rustheap_enemy19.setSoulStoneDropChance(4);
	easy_rustheap_enemy19.setDescription("A tiny mechanical gecko, possibly not alive in any traditional sense. It moves in jerky, mechanical ticks. No one knows who built it or why. It attacks on reflex.");
	CombatLocation easy_rustheap_clocation19("Rustheap Foothills, Springfracture Ridge", &easy_rustheap_enemy19);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation19);
	
	//Rustheap | Easy Enemy no. 20 | Ozone Runt
	// Fast electric skirmisher. Very low HP, punishes slow play. Drop table expanded.
	// Fixed: enemy name corrected from "Ozone Rust" to "Ozone Runt".
	Spell easy_rustheap_enemy20_spell1("Static Surge", "The user releases a sudden burst of built-up static, striking the target with a sharp electric discharge.", 10, 2, "Electricity");
	Spell* easy_rustheap_enemy20_spells[4] = {&easy_rustheap_enemy20_spell1, nullptr, nullptr, nullptr};
	Enemy easy_rustheap_enemy20("Ozone Runt", 28, 4, 5, 9, 15, 2.0, easy_rustheap_enemy20_spells);
	Consumable easy_rustheap_enemy20_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Scroll easy_rustheap_enemy20_drop2("", &easy_rustheap_enemy20_spell1);
	Weapon easy_rustheap_enemy20_drop3("Charge-Coil Rod", "Club", "A short insulated rod with a crackling copper tip. Touch it wrong and it bites back.", 4);
	easy_rustheap_enemy20.addReward(&easy_rustheap_enemy20_drop1, 60, 3);
	easy_rustheap_enemy20.addReward(&easy_rustheap_enemy20_drop2, 25, 1);
	easy_rustheap_enemy20.addReward(&easy_rustheap_enemy20_drop3, 20, 1);
	easy_rustheap_enemy20.setSoulStoneDropChance(3);
	easy_rustheap_enemy20.setDescription("A raccoon who brushed against the Tesla-fields too many times as a child. Their fur stands permanently on end, twitching with residual charge. Speech comes in broken fragments, and the air around them carries the sharp scent of ozone.");
	CombatLocation easy_rustheap_clocation20("Rustheap Foothills, Ozone Drift", &easy_rustheap_enemy20);
	easy_rustheap_locations.push_back(&easy_rustheap_clocation20);
	
	// =============================================
	//  RUSTHEAP EASY TREASURES
	// =============================================
	
	//Rustheap | Easy Treasure no. 1 | The Collapsed Watchpost
	// Metal-flavored loot: weapon + consumable + armor scroll chance.
	Weapon easy_rustheap_treasure1_weapon("Dented Watchman's Baton", "Club", "A heavy enforcement baton, its surface pitted with rust. It still swings true.", 5.0);
	Consumable easy_rustheap_treasure1_consumable("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10.0);
	Scroll easy_rustheap_treasure1_scroll("", &easy_rustheap_enemy1_spell1);
	TreasureLocation easy_rustheap_tlocation1("The Collapsed Watchpost", "Dented Security Lockbox");
	easy_rustheap_tlocation1.addReward(&easy_rustheap_treasure1_weapon, 45, 1);
	easy_rustheap_tlocation1.addReward(&easy_rustheap_treasure1_consumable, 70, 2);
	easy_rustheap_tlocation1.addReward(&easy_rustheap_treasure1_scroll, 30, 1);
	easy_rustheap_tlocation1.setSoulStoneDropChance(8);
	easy_rustheap_tlocation1.setXpReward(25);
	easy_rustheap_locations.push_back(&easy_rustheap_tlocation1);
	
	//Rustheap | Easy Treasure no. 2 | The Gutted Engineer's Cache
	// Armor-heavy cache; rewards players who explore past the wire ravine.
	Armor easy_rustheap_treasure2_armor("Grease-Slicked Iron Vest", "A padded vest reinforced with sheets of scrap metal and soaked in machine oil. Flexible enough to still move in.", 6.5, 2);
	Consumable easy_rustheap_treasure2_consumable("Firecore", "A pulsing, heat-warped core that radiates unstable warmth.", 20.0);
	Scroll easy_rustheap_treasure2_scroll("", &easy_rustheap_enemy5_spell2);
	TreasureLocation easy_rustheap_tlocation2("The Gutted Engineer's Cache", "Oil-Sealed Toolchest");
	easy_rustheap_tlocation2.addReward(&easy_rustheap_treasure2_armor, 40, 1);
	easy_rustheap_tlocation2.addReward(&easy_rustheap_treasure2_consumable, 60, 2);
	easy_rustheap_tlocation2.addReward(&easy_rustheap_treasure2_scroll, 25, 1);
	easy_rustheap_tlocation2.setSoulStoneDropChance(9);
	easy_rustheap_tlocation2.setXpReward(25);
	easy_rustheap_locations.push_back(&easy_rustheap_tlocation2);
	
	//Rustheap | Easy Treasure no. 3 | The Furnace Relic Pit
	// Fire-themed; best weapon in easy tier. Hidden near Bilespew territory.
	Weapon easy_rustheap_treasure3_weapon("Slag-Tempered Cleaver", "Sword", "A broad cleaver reforged in furnace slag. The metal has a slight orange tinge from the heat it absorbed.", 6.5);
	Consumable easy_rustheap_treasure3_consumable1("Burrower's Gristle", "Dense, chewy meat scraped from inside the worm-construct. It does not taste like anything natural.", 12.5);
	Consumable easy_rustheap_treasure3_consumable2("Greased Ration Block", "A dense, oil-slick block of pressed protein from the heap's lower mess tins.", 10.0);
	Scroll easy_rustheap_treasure3_scroll("", &easy_rustheap_enemy17_spell2);
	TreasureLocation easy_rustheap_tlocation3("The Furnace Relic Pit", "Heat-Warped Iron Chest");
	easy_rustheap_tlocation3.addReward(&easy_rustheap_treasure3_weapon, 40, 1);
	easy_rustheap_tlocation3.addReward(&easy_rustheap_treasure3_consumable1, 55, 2);
	easy_rustheap_tlocation3.addReward(&easy_rustheap_treasure3_consumable2, 45, 1);
	easy_rustheap_tlocation3.addReward(&easy_rustheap_treasure3_scroll, 20, 1);
	easy_rustheap_tlocation3.setSoulStoneDropChance(11);
	easy_rustheap_tlocation3.setXpReward(25);
	easy_rustheap_locations.push_back(&easy_rustheap_tlocation3);
	
	// =============================================
	//  RUSTHEAP MEDIUM ENEMIES
	// =============================================
	
	//Rustheap | Medium Enemy no. 1 | Cindermark
	// Boss-level Snailsville equivalent: high dmg, 3 explosive spells, speed 5.
	// Blaststream (13, CD3) is the workhorse; Detonation Fist (16, CD5) is the spike.
	Spell medium_rustheap_enemy1_spell1("Detonation Fist", "The user propels themselves forward with a blast, delivering a punch that detonates violently on impact.", 16, 5, "Explosion");
	Spell medium_rustheap_enemy1_spell2("Blaststream", "The user slams their palms together, unleashing a continuous surge of explosions that roars forward like a living inferno.", 13, 3, "Explosion");
	Spell medium_rustheap_enemy1_spell3("Flashpoint Grip", "The user reaches for the target's face — and ignites a point-blank explosion the moment contact is made.", 12, 2, "Explosion");
	Spell* medium_rustheap_enemy1_spells[4] = {&medium_rustheap_enemy1_spell1, &medium_rustheap_enemy1_spell2, &medium_rustheap_enemy1_spell3, nullptr};
	Enemy medium_rustheap_enemy1("Cindermark", 60, 12, 7, 5, 15, 1.25, medium_rustheap_enemy1_spells);
	Scroll medium_rustheap_enemy1_drop1("", &medium_rustheap_enemy1_spell1);
	Scroll medium_rustheap_enemy1_drop2("", &medium_rustheap_enemy1_spell2);
	Scroll medium_rustheap_enemy1_drop3("", &medium_rustheap_enemy1_spell3);
	Armor medium_rustheap_enemy1_drop4("Blastproof Helmet", "A helmet forged from layered blast-resistant plating.", 5, 1);
	Armor medium_rustheap_enemy1_drop5("Blastproof Vest", "A heavy vest designed to absorb shockwaves and shrapnel from repeated detonations.", 10, 2);
	Weapon medium_rustheap_enemy1_drop6("Blastknuckle Gauntlets", "Explosive gloves", "Reinforced gauntlets rigged to detonate on impact, each strike carrying controlled destruction.", 7);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop1, 25, 1);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop2, 25, 1);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop3, 25, 1);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop4, 25, 1);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop5, 25, 1);
	medium_rustheap_enemy1.addReward(&medium_rustheap_enemy1_drop6, 15, 1);
	medium_rustheap_enemy1.setSoulStoneDropChance(10);
	medium_rustheap_enemy1.setDescription("A veteran of Rustheap's constant internecine wars, their chest scarred with shrapnel burns — most of them self-inflicted. They no longer flinch at the sound of explosions. Collateral damage stopped mattering long ago.");
	CombatLocation medium_rustheap_clocation1("Rustheap Lower Slopes, Blastscar Front", &medium_rustheap_enemy1);
	vector<Location*> medium_rustheap_locations = {&medium_rustheap_clocation1};
	
	//Rustheap | Medium Enemy no. 2 | Slaghand
	// Slow and brutal: speed 3, dmg 13. Molten Slag Burst DOT (-6/tick x5) is the kill condition if it lands.
	HpStatusEffect medium_rustheap_enemy2_status1("Searing Burn", 5, 2, -6);
	StatusEffectSpell medium_rustheap_enemy2_spell1("Molten Slag Burst", "The user hurls a chunk of superheated metal, splashing molten fragments that cling and burn.", 18, 5, 65, &medium_rustheap_enemy2_status1, "Molten metal");
	Spell medium_rustheap_enemy2_spell2("Rustcleaver", "A heavy, practiced slash driven with brutal efficiency.", 12, 2, "Slash");
	Spell* medium_rustheap_enemy2_spells[4] = {&medium_rustheap_enemy2_spell1, &medium_rustheap_enemy2_spell2, nullptr, nullptr};
	Enemy medium_rustheap_enemy2("Slaghand", 65, 13, 7, 3, 15, 1.4, medium_rustheap_enemy2_spells);
	Consumable medium_rustheap_enemy2_drop1("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15);
	Scroll medium_rustheap_enemy2_drop2("", &medium_rustheap_enemy2_spell1);
	Scroll medium_rustheap_enemy2_drop3("", &medium_rustheap_enemy2_spell2);
	medium_rustheap_enemy2.addReward(&medium_rustheap_enemy2_drop1, 50, 3);
	medium_rustheap_enemy2.addReward(&medium_rustheap_enemy2_drop2, 25, 1);
	medium_rustheap_enemy2.addReward(&medium_rustheap_enemy2_drop3, 25, 1);
	medium_rustheap_enemy2.setSoulStoneDropChance(13);
	medium_rustheap_enemy2.setDescription("A raccoon who works the heap's illegal forges, their hands encased in hardened slag and scar tissue. Every throw of molten metal feels practiced \x97 rhythmic, almost ritualistic. The heat never seems to bother them.");
	CombatLocation medium_rustheap_clocation2("Rustheap Lower Slopes, Slagflow Works", &medium_rustheap_enemy2);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation2);
	
	//Rustheap | Medium Enemy no. 3 | Embervein
	// Fast and explosive: speed 8, crit 20, critVal 2. Scaleburst Barrage avg 14x1=14 dmg — devastating spike.
	// Ragefire DOT (-8.5/tick x6) is potentially fight-ending if it procs.
	HpStatusEffect medium_rustheap_enemy3_status1("Hatred Flame", 6, 3, -8.5);
	StatusEffectSpell medium_rustheap_enemy3_spell1("Ragefire Burst", "The user unleashes a massive fireball, dense with heat and fury, that erupts on impact.", 16, 4, 85, &medium_rustheap_enemy3_status1, "Fire");
	Spell medium_rustheap_enemy3_spell2("Molten Impact", "A brutal slam infused with heat, scorching the ground and leaving behind a smoldering crater.", 20, 5, "Fire");
	ChainingSpell medium_rustheap_enemy3_spell3("Scaleburst Barrage", "The user fires a relentless storm of molten, blade-like scales that tear into the target in rapid succession.", 1, 4, 10, 18, "Fire");
	Spell* medium_rustheap_enemy3_spells[4] = {&medium_rustheap_enemy3_spell1, &medium_rustheap_enemy3_spell2, &medium_rustheap_enemy3_spell3, nullptr};
	Enemy medium_rustheap_enemy3("Embervein", 60, 9, 7, 8, 20, 2, medium_rustheap_enemy3_spells);
	Consumable medium_rustheap_enemy3_drop1("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20);
	Scroll medium_rustheap_enemy3_drop2("", &medium_rustheap_enemy3_spell1);
	Scroll medium_rustheap_enemy3_drop3("", &medium_rustheap_enemy3_spell2);
	Armor medium_rustheap_enemy3_drop4("Gecko Skin Boots", "Boots crafted from heat-hardened gecko hide.", 5, 5);
	Armor medium_rustheap_enemy3_drop5("Gecko Skin Gloves", "Gloves made from treated gecko skin, resistant to extreme temperatures.", 5, 3);
	medium_rustheap_enemy3.addReward(&medium_rustheap_enemy3_drop1, 50, 3);
	medium_rustheap_enemy3.addReward(&medium_rustheap_enemy3_drop2, 25, 1);
	medium_rustheap_enemy3.addReward(&medium_rustheap_enemy3_drop3, 25, 1);
	medium_rustheap_enemy3.addReward(&medium_rustheap_enemy3_drop4, 25, 1);
	medium_rustheap_enemy3.addReward(&medium_rustheap_enemy3_drop5, 25, 1);
	medium_rustheap_enemy3.setSoulStoneDropChance(12);
	medium_rustheap_enemy3.setDescription("A gecko whose internal temperature runs dangerously high. Molten channels pulse beneath thinning scales, glowing faintly with every heartbeat. When its tail strikes the ground, the impact leaves scorched craters \x97 as if the heat within is constantly trying to escape.");
	CombatLocation medium_rustheap_clocation3("Rustheap Lower Slopes, Emberfracture Basin", &medium_rustheap_enemy3);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation3);
	
	//Rustheap | Medium Enemy no. 4 | Cleavergraft
	// Slowest medium (speed 2), highest dmg (15). Player always goes first but every trade hurts badly.
	// Molten Pierce (12, CD2) fires constantly. Grafted Uppercut (15, CD4) is the punish.
	Spell medium_rustheap_enemy4_spell1("Grafted Uppercut", "The user rapidly forges a crude graft over their arm and drives it upward in a devastating, reinforced strike.", 15, 4, "Metal");
	Spell medium_rustheap_enemy4_spell2("Molten Pierce", "The user shapes a spear from dripping molten metal and hurls it forward before it fully solidifies.", 12, 2, "Molten metal");
	Spell* medium_rustheap_enemy4_spells[4] = {&medium_rustheap_enemy4_spell1, &medium_rustheap_enemy4_spell2, nullptr, nullptr};
	Enemy medium_rustheap_enemy4("Cleavergraft", 68, 15, 7, 2, 15, 1.1, medium_rustheap_enemy4_spells);
	Scroll medium_rustheap_enemy4_drop1("", &medium_rustheap_enemy4_spell1);
	Scroll medium_rustheap_enemy4_drop2("", &medium_rustheap_enemy4_spell2);
	Armor medium_rustheap_enemy4_drop3("Oil-Stained Overalls", "Heavy work overalls stiff with grease, ash, and something darker that never washed out.", 7, 4);
	Weapon medium_rustheap_enemy4_drop4("Rustbound Graft", "Graft", "A jagged mechanical limb torn from Cleavergraft's body, still faintly twitching.", 8);
	medium_rustheap_enemy4.addReward(&medium_rustheap_enemy4_drop1, 25, 1);
	medium_rustheap_enemy4.addReward(&medium_rustheap_enemy4_drop2, 25, 1);
	medium_rustheap_enemy4.addReward(&medium_rustheap_enemy4_drop3, 25, 1);
	medium_rustheap_enemy4.addReward(&medium_rustheap_enemy4_drop4, 15, 1);
	medium_rustheap_enemy4.setSoulStoneDropChance(15);
	medium_rustheap_enemy4.setDescription("A veteran Rustheap engineer who replaced a lost hand with a crude graft of metal and scrap. The limb grows, reshapes, and breaks apart mid-combat. Everything they build looks unfinished \x97 and everything they build still works.");
	CombatLocation medium_rustheap_clocation4("Rustheap Lower Slopes, Graftwork Yard", &medium_rustheap_enemy4);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation4);
	
	//Rustheap | Medium Enemy no. 5 | The Rebuilt
	// Fast (speed 8), Shards of Ruin avg 14 hits x 0.5 = 7 dmg — relentless chip.
	// Wailing Horn taunt forces the player to reckon with it directly.
	Spell medium_rustheap_enemy5_spell1("Ruinous Impact", "The entity surges forward as if guided by unseen force, its form collapsing into a catastrophic impact that erupts in violent destruction.", 16, 5, "Explosion");
	TauntStatusEffect medium_rustheap_enemy5_status1("Dread Call", 3, 1);
	StatusEffectSpell medium_rustheap_enemy5_spell2("Wailing Horn", "A hollow, unnatural horn-call echoes through the air, compelling the target to face the source of the sound.", 12, 4, 75, &medium_rustheap_enemy5_status1);
	ChainingSpell medium_rustheap_enemy5_spell3("Shards of Ruin", "A storm of jagged, glass-like fragments tears through the air, lashing the target in relentless succession.", 0.5, 6, 14, 34, "Glass");
	Spell* medium_rustheap_enemy5_spells[4] = {&medium_rustheap_enemy5_spell1, &medium_rustheap_enemy5_spell2, &medium_rustheap_enemy5_spell3, nullptr};
	Enemy medium_rustheap_enemy5("The Rebuilt", 67, 9, 7, 8, 20, 1.5, medium_rustheap_enemy5_spells);
	Scroll medium_rustheap_enemy5_drop1("", &medium_rustheap_enemy5_spell1);
	Scroll medium_rustheap_enemy5_drop2("", &medium_rustheap_enemy5_spell2);
	Scroll medium_rustheap_enemy5_drop3("", &medium_rustheap_enemy5_spell3);
	medium_rustheap_enemy5.addReward(&medium_rustheap_enemy5_drop1, 25, 1);
	medium_rustheap_enemy5.addReward(&medium_rustheap_enemy5_drop2, 25, 1);
	medium_rustheap_enemy5.addReward(&medium_rustheap_enemy5_drop3, 20, 1);
	medium_rustheap_enemy5.setSoulStoneDropChance(16);
	medium_rustheap_enemy5.setDescription("A thing that should have remained broken. Twisted metal and bone fused together, rebuilt again and again by unseen hands. It remembers destruction \x97 not as pain, but as purpose. Each movement feels like the echo of a long-forgotten catastrophe.");
	CombatLocation medium_rustheap_clocation5("Rustheap Lower Slopes, Field of Broken Echoes", &medium_rustheap_enemy5);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation5);
	
	//Rustheap | Medium Enemy no. 6 | Stirrupbone
	TauntStatusEffect medium_rustheap_enemy6_status1("Blinded by Soot", 4, 1);
	StatusEffectSpell medium_rustheap_enemy6_spell1("Ashen Drift", "The user exhales a choking veil of ash and smoke, obscuring vision and disorienting the target.", 12, 5, 80, &medium_rustheap_enemy6_status1, "Smoke");
	Spell medium_rustheap_enemy6_spell2("Rusted Wheel Crush", "A heavy, rust-bound wheel is driven into the target with crushing force, as if part of the user’s own body.", 10, 3, "Physical");
	Spell* medium_rustheap_enemy6_spells[4] = {&medium_rustheap_enemy6_spell1, &medium_rustheap_enemy6_spell2, nullptr, nullptr};
	Enemy medium_rustheap_enemy6("Stirrupbone", 70, 11, 7, 8, 10, 1.5, medium_rustheap_enemy6_spells);
	Consumable medium_rustheap_enemy6_drop1("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15);
	Scroll medium_rustheap_enemy6_drop2("", &medium_rustheap_enemy6_spell1);
	Scroll medium_rustheap_enemy6_drop3("", &medium_rustheap_enemy6_spell2);
	medium_rustheap_enemy6.addReward(&medium_rustheap_enemy6_drop1, 50, 3);
	medium_rustheap_enemy6.addReward(&medium_rustheap_enemy6_drop2, 25, 1);
	medium_rustheap_enemy6.addReward(&medium_rustheap_enemy6_drop3, 25, 1);
	medium_rustheap_enemy6.setSoulStoneDropChance(12);
	medium_rustheap_enemy6.setDescription("A raccoon whose lower body has long since been replaced by iron stirrups and a fused frame of wheels. They no longer walk — only drag or ride. When mounted, their movements become unnaturally fluid, as if guided by something older than instinct.");
	CombatLocation medium_rustheap_clocation6("Rustheap Lower Slopes, Ashen Trackway", &medium_rustheap_enemy6);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation6);
	
	//Rustheap | Medium Enemy no. 7 | Wrongfit
	Spell medium_rustheap_enemy7_spell1("Misfired Relic Cannon", "The user forces a malformed cannon from their own grafted mass and unleashes a violent, unstable blast.", 14, 5, "Metal");
	Spell medium_rustheap_enemy7_spell2("Nerve Surge Collapse", "The user's mismatched implants surge uncontrollably, releasing erratic bursts of energy that lash outward.", 12, 4, "Electric");
	ChainingSpell medium_rustheap_enemy7_spell3("Jagged Gear Rain", "A flurry of twisted, half-formed metal shards erupts and launches forward like a storm of broken mechanisms.", 2, 4, 2, 6, "Metal");
	Spell* medium_rustheap_enemy7_spells[4] = {&medium_rustheap_enemy7_spell1, &medium_rustheap_enemy7_spell2, &medium_rustheap_enemy7_spell3, nullptr};
	Enemy medium_rustheap_enemy7("Wrongfit", 62, 13, 7, 9, 20, 1.25, medium_rustheap_enemy7_spells);
	Consumable medium_rustheap_enemy7_drop1("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15);
	Scroll medium_rustheap_enemy7_drop2("", &medium_rustheap_enemy7_spell1);
	Scroll medium_rustheap_enemy7_drop3("", &medium_rustheap_enemy7_spell2);
	Scroll medium_rustheap_enemy7_drop4("", &medium_rustheap_enemy7_spell3);
	Weapon medium_rustheap_enemy7_drop5("Malformed Gearblade", "Sword", "A warped blade of fused gears that hums with unstable tension.", 7.5);
	medium_rustheap_enemy7.addReward(&medium_rustheap_enemy7_drop1, 50, 3);
	medium_rustheap_enemy7.addReward(&medium_rustheap_enemy7_drop2, 25, 1);
	medium_rustheap_enemy7.addReward(&medium_rustheap_enemy7_drop3, 25, 1);
	medium_rustheap_enemy7.addReward(&medium_rustheap_enemy7_drop4, 25, 1);
	medium_rustheap_enemy7.addReward(&medium_rustheap_enemy7_drop5, 20, 1);
	medium_rustheap_enemy7.setSoulStoneDropChance(15);
	medium_rustheap_enemy7.setDescription("A raccoon who carved and forced foreign machinery into their own body without knowledge or restraint. Nothing aligns. Nothing fits. Yet it lives — sustained by pain, rage, and something that refuses to let the body fail.");
	CombatLocation medium_rustheap_clocation7("Rustheap Lower Slopes, Misaligned Workshop", &medium_rustheap_enemy7);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation7);
	
	//Rustheap | Medium Enemy no. 8 | Patience-of-Ash
	TauntStatusEffect medium_rustheap_enemy8_status1("Fractured plating", 4, 2);
	StatusEffectSpell medium_rustheap_enemy8_spell1("Cinder Implosion", "The user pinches the air itself, birthing a trembling core of unstable heat that collapses inward before violently erupting toward the target.", 12, 4, 80, &medium_rustheap_enemy8_status1, "Explosion");
	ChainingSpell medium_rustheap_enemy8_spell2("Ashen Finger Volley", "The user raises a trembling hand, each finger discharging rapid bursts of volatile force like a ritual gone wrong.", 1.5, 4, 3, 9, "Explosion");
	Spell medium_rustheap_enemy8_spell3("Detonation March", "Each step cracks the ground with suppressed blasts as the user advances, culminating in a devastating point-blank eruption upon contact.", 16, 5, "Explosion");
	Spell* medium_rustheap_enemy8_spells[4] = {&medium_rustheap_enemy8_spell1, &medium_rustheap_enemy8_spell2, nullptr, nullptr};
	Enemy medium_rustheap_enemy8("Patience-of-Ash", 65, 7, 7, 3, 20, 2, medium_rustheap_enemy8_spells);
	Armor medium_rustheap_enemy8_drop1("Blastwarden Helm", "A scorched helmet worn by those who stand too close to controlled annihilation.", 5.5, 1);
	Scroll medium_rustheap_enemy8_drop2("", &medium_rustheap_enemy8_spell1);
	Scroll medium_rustheap_enemy8_drop3("", &medium_rustheap_enemy8_spell2);
	Scroll medium_rustheap_enemy8_drop4("", &medium_rustheap_enemy8_spell3);
	medium_rustheap_enemy8.addReward(&medium_rustheap_enemy8_drop1, 25, 1);
	medium_rustheap_enemy8.addReward(&medium_rustheap_enemy8_drop2, 25, 1);
	medium_rustheap_enemy8.addReward(&medium_rustheap_enemy8_drop3, 25, 1);
	medium_rustheap_enemy8.addReward(&medium_rustheap_enemy8_drop4, 25, 1);
	medium_rustheap_enemy8.setSoulStoneDropChance(13);
	medium_rustheap_enemy8.setDescription("A demolition specialist who doesn't rush. Sets charges with the calm of someone who has accepted all possible outcomes. The countdown is just ceremony.");
	CombatLocation medium_rustheap_clocation8("Rustheap Lower Slopes, Ashmarked Detonation Grounds", &medium_rustheap_enemy8);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation8);
	
	//Rustheap | Medium Enemy no. 9 | Ironscald
	Spell medium_rustheap_enemy9_spell1("Scalding Brandblade", "The user tears molten metal from their own plating, shaping a dripping blade that screams as it cuts through the air.", 13, 3, "Molten metal");
	ChainingSpell medium_rustheap_enemy9_spell2("Molten Dripstorm", "Globs of liquefied metal spit outward in rapid bursts, clinging and burning as they eat into the target.", 1, 3, 5, 13, "Molten metal");
	Spell* medium_rustheap_enemy9_spells[4] = {&medium_rustheap_enemy9_spell1, &medium_rustheap_enemy9_spell2, nullptr, nullptr};
	Enemy medium_rustheap_enemy9("Ironscald", 62, 6, 7, 8, 20, 2.5, medium_rustheap_enemy9_spells);
	Consumable medium_rustheap_enemy9_drop1("Quality Gecko Meat", "Unusually rich flesh, still warm, threaded with residual heat.", 20);
	Armor medium_rustheap_enemy9_drop2("Sloughing Steel Carapace", "Molten plating that never fully solidifies, constantly shifting and threatening to collapse off the body.", 8, 2);
	Armor medium_rustheap_enemy9_drop3("Dripsteel Greaves", "Leg armor that seeps liquid heat with every movement, leaving scorched traces behind.", 6, 4);
	Scroll medium_rustheap_enemy9_drop4("", &medium_rustheap_enemy9_spell1);
	Scroll medium_rustheap_enemy9_drop5("", &medium_rustheap_enemy9_spell2);
	medium_rustheap_enemy9.addReward(&medium_rustheap_enemy9_drop1, 50, 3);
	medium_rustheap_enemy9.addReward(&medium_rustheap_enemy9_drop2, 25, 1);
	medium_rustheap_enemy9.addReward(&medium_rustheap_enemy9_drop3, 25, 1);
	medium_rustheap_enemy9.addReward(&medium_rustheap_enemy9_drop4, 25, 1);
	medium_rustheap_enemy9.addReward(&medium_rustheap_enemy9_drop5, 25, 1);
	medium_rustheap_enemy9.setSoulStoneDropChance(16);
	medium_rustheap_enemy9.setDescription("A gecko whose body has been deliberately plated with poured steel over living scales. The process is agonizing. The result is a creature that shrugs off blades and radiates heat.");
	CombatLocation medium_rustheap_clocation9("Rustheap Lower Slopes, Scalded Plating Grounds", &medium_rustheap_enemy9);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation9);
	
	//Rustheap | Medium Enemy no. 10 | Dredgepull
	Spell medium_rustheap_enemy10_spell1("Ferrum Grasp", "The user seizes the iron within the target’s blood, violently dragging their body across the ground as if pulled by an unseen god.", 16, 6, "Magnetic");
	Spell medium_rustheap_enemy10_spell2("Polarity Collapse", "Scrap and rust spiral into the user’s fist, forming a dense mass before polarity flips on impact, tearing the construct apart in a violent burst.", 14, 4, "Magnetic");
	ArmorBreakStatusEffect medium_rustheap_enemy10_status1("Stripped Plating", 5, 1);
	StatusEffectSpell medium_rustheap_enemy10_spell3("Gravepull Disarm", "A cruel magnetic surge rips armor from the target’s body, casting it aside like shed skin.", 12, 4, 90, &medium_rustheap_enemy10_status1, "Magnetic");
	Spell* medium_rustheap_enemy10_spells[4] = {&medium_rustheap_enemy10_spell1, &medium_rustheap_enemy10_spell2, &medium_rustheap_enemy10_spell3, nullptr};
	Enemy medium_rustheap_enemy10("Dredgepull", 72, 13, 7, 3, 25, 1.15, medium_rustheap_enemy10_spells);
	Consumable medium_rustheap_enemy10_drop1("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15);
	Scroll medium_rustheap_enemy10_drop2("", &medium_rustheap_enemy10_spell1);
	Scroll medium_rustheap_enemy10_drop3("", &medium_rustheap_enemy10_spell2);
	Scroll medium_rustheap_enemy10_drop4("", &medium_rustheap_enemy10_spell3);
	medium_rustheap_enemy10.addReward(&medium_rustheap_enemy10_drop1, 50, 3);
	medium_rustheap_enemy10.addReward(&medium_rustheap_enemy10_drop2, 25, 1);
	medium_rustheap_enemy10.addReward(&medium_rustheap_enemy10_drop3, 25, 1);
	medium_rustheap_enemy10.addReward(&medium_rustheap_enemy10_drop4, 25, 1);
	medium_rustheap_enemy10.setSoulStoneDropChance(17);
	medium_rustheap_enemy10.setDescription("A raccoon with magnetic implants welded into their forearms, pulling shrapnel and refuse into orbit like a grotesque extension of themselves. What they pull, they use to crush.");
	CombatLocation medium_rustheap_clocation10("Rustheap Lower Slopes, Ironwake Dredge Fields", &medium_rustheap_enemy10);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation10);
	
	//Rustheap | Medium Enemy no. 11 | Bloodfuel
	ChainingSpell medium_rustheap_enemy11_spell1("Rupture Chain", "Each motion tears open volatile veins within the body, releasing chained bursts of unstable force that detonate in erratic succession.", 5, 5, 1, 3, "Explosive");
	Spell medium_rustheap_enemy11_spell2("Detonation Breath", "The user exhales a pressurized stream of ignition, each pulse erupting into short-lived but violent explosions.", 13, 3, "Explosive");
	ChainingSpell medium_rustheap_enemy11_spell3("Ashen Gunsign", "With hollow gestures of a forgotten ritual, the user snaps their fingers into existence, firing bursts of searing force as if the air itself obeyed.", 1, 5, 8, 15, "Fire"); //ONE PIEEECEEEE IS REAAAAAAAAAL
	Spell* medium_rustheap_enemy11_spells[4] = {&medium_rustheap_enemy11_spell1, &medium_rustheap_enemy11_spell2, &medium_rustheap_enemy11_spell3, nullptr};
	Enemy medium_rustheap_enemy11("Spade, the Bloodfuel", 67, 8, 7, 4, 15, 2.2, medium_rustheap_enemy11_spells); //cant call him Ace, too obvi
	Consumable medium_rustheap_enemy11_drop1("Bunch of Raccoon Flesh", "A heap of stringy raccoon meat, roughly torn and still warm.", 15);
	Scroll medium_rustheap_enemy11_drop2("", &medium_rustheap_enemy11_spell1);
	Scroll medium_rustheap_enemy11_drop3("", &medium_rustheap_enemy11_spell2);
	Scroll medium_rustheap_enemy11_drop4("", &medium_rustheap_enemy11_spell3);
	Armor medium_rustheap_enemy11_drop5("Grinning Ember Mask", "A warped, smiling helm stained with soot and heat — its grin never fades, no matter how long it's worn.", 5, 1); //ONE PIECEEEE IS REAAAAL; also not one of the strongest helmet drops from this batch of enemies
	medium_rustheap_enemy11.addReward(&medium_rustheap_enemy11_drop1, 50, 3);
	medium_rustheap_enemy11.addReward(&medium_rustheap_enemy11_drop2, 25, 1);
	medium_rustheap_enemy11.addReward(&medium_rustheap_enemy11_drop3, 25, 1);
	medium_rustheap_enemy11.addReward(&medium_rustheap_enemy11_drop4, 25, 1);
	medium_rustheap_enemy11.addReward(&medium_rustheap_enemy11_drop5, 25, 1);
	medium_rustheap_enemy11.setSoulStoneDropChance(12);
	medium_rustheap_enemy11.setDescription("A raccoon who channels fire not through a device, but through their body — blood replaced with a combustible compound, voluntarily. The pain is religious to them.");
	CombatLocation medium_rustheap_clocation11("Rustheap Lower Slopes, Bloodfuel Pyre Grounds", &medium_rustheap_enemy11);
	medium_rustheap_locations.push_back(&medium_rustheap_clocation11);
	
	// =============================================
	//  RUSTHEAP HARD ENEMIES
	// =============================================
	
	//Rustheap | Hard Enemy no. 1 | Gravestring
	// Elite musician-killer. Speed 6 goes first. Resonance Shred is the sustained DoT threat (-4/tick x5 = -20 HP).
	// Chord Collapse (20, CD5) is the spike. Staccato Shred avg 8x1.5=12 dmg — the punishing chain.
	// Player needs to burst this down fast before DoT stacks.
	HpStatusEffect hard_rustheap_enemy1_status1("Resonance Bleed", 5, 2, -4);
	StatusEffectSpell hard_rustheap_enemy1_spell1("Resonance Shred", "The guitar's blades spin at resonant frequency, tearing flesh with sound and steel simultaneously.", 14, 4, 75, &hard_rustheap_enemy1_status1, "Sound");
	Spell hard_rustheap_enemy1_spell2("Chord Collapse", "Every string pulled at once — the resulting shockwave caves in whatever stands in front of it.", 20, 5, "Sound");
	ChainingSpell hard_rustheap_enemy1_spell3("Staccato Shred", "Short, brutal bursts of resonance-blade strikes, each note a new wound.", 1.5, 4, 6, 10, "Sound");
	Spell* hard_rustheap_enemy1_spells[4] = {&hard_rustheap_enemy1_spell1, &hard_rustheap_enemy1_spell2, &hard_rustheap_enemy1_spell3, nullptr};
	Enemy hard_rustheap_enemy1("Gravestring", 80, 10, 8, 6, 18, 1.5, hard_rustheap_enemy1_spells);
	Consumable hard_rustheap_enemy1_drop1("Wire-Fed Rat Flesh", "Stringy meat laced with thin copper filaments that still faintly hum with residual charge.", 9.5);
	Scroll hard_rustheap_enemy1_drop2("", &hard_rustheap_enemy1_spell1);
	Scroll hard_rustheap_enemy1_drop3("", &hard_rustheap_enemy1_spell2);
	Weapon hard_rustheap_enemy1_drop4("Resonance Blade Guitar", "Guitar", "A guitar with vibrating resonance blades along each string. Every chord draws blood.", 9);
	Armor hard_rustheap_enemy1_drop5("Scrap-Studded Stage Coat", "A long coat riveted with scrap metal along the lapels. Worn through more performances than most survive.", 7, 2);
	hard_rustheap_enemy1.addReward(&hard_rustheap_enemy1_drop1, 40, 2);
	hard_rustheap_enemy1.addReward(&hard_rustheap_enemy1_drop2, 20, 1);
	hard_rustheap_enemy1.addReward(&hard_rustheap_enemy1_drop3, 20, 1);
	hard_rustheap_enemy1.addReward(&hard_rustheap_enemy1_drop4, 15, 1);
	hard_rustheap_enemy1.addReward(&hard_rustheap_enemy1_drop5, 20, 1);
	hard_rustheap_enemy1.setSoulStoneDropChance(22);
	hard_rustheap_enemy1.setDescription("An elite raccoon musician turned killer, guitar fitted with resonance blades. Every chord peels plating, ruptures tissue. The performance continues through the screaming.");
	CombatLocation hard_rustheap_clocation1("Rustheap Upper Slopes, The Shredded Stage", &hard_rustheap_enemy1);
	vector<Location*> hard_rustheap_locations = {&hard_rustheap_clocation1};
	
	//Rustheap | Hard Enemy no. 2 | Earthcrack
	// Slow drummer (speed 3), player goes first — compensated by monstrous HP and spell power.
	// Seismic Cascade avg 12x2=24 potential dmg — the most dangerous chaining spell in the game so far.
	// Magnitude Slam (22, CD5) is the single biggest direct hit outside bosses.
	// Tectonic Pulse DoT (-5/tick x4) can stack with chain damage for an overwhelming pressure fight.
	HpStatusEffect hard_rustheap_enemy2_status1("Ground Tremor", 4, 2, -5);
	StatusEffectSpell hard_rustheap_enemy2_spell1("Tectonic Pulse", "A sustained rhythm drives shockwaves deep into the earth, the vibrations travelling up through bone.", 16, 4, 70, &hard_rustheap_enemy2_status1, "Physical");
	Spell hard_rustheap_enemy2_spell2("Magnitude Slam", "The drummer brings both sticks down simultaneously with seismic force. The ground does not recover.", 22, 5, "Physical");
	ChainingSpell hard_rustheap_enemy2_spell3("Seismic Cascade", "A relentless drumroll that sends shockwave after shockwave through the target — each beat harder than the last.", 2, 4, 8, 14, "Physical");
	Spell* hard_rustheap_enemy2_spells[4] = {&hard_rustheap_enemy2_spell1, &hard_rustheap_enemy2_spell2, &hard_rustheap_enemy2_spell3, nullptr};
	Enemy hard_rustheap_enemy2("Earthcrack", 85, 11, 8, 3, 16, 1.5, hard_rustheap_enemy2_spells);
	Consumable hard_rustheap_enemy2_drop1("Shardscale Gecko Meat", "Dense flesh threaded with metallic splinters from years spent in the rustheap.", 15);
	Scroll hard_rustheap_enemy2_drop2("", &hard_rustheap_enemy2_spell1);
	Scroll hard_rustheap_enemy2_drop3("", &hard_rustheap_enemy2_spell2);
	Weapon hard_rustheap_enemy2_drop4("Earthcrack Drumsticks", "Club", "A pair of percussion rods wrapped in iron and used to beat the earth into submission. Each one weighs more than it should.", 9.5);
	Armor hard_rustheap_enemy2_drop5("Seismic Plated Boots", "Boots reinforced to withstand the kickback of sustained ground-percussion. The soles are cracked from use.", 7, 5);
	hard_rustheap_enemy2.addReward(&hard_rustheap_enemy2_drop1, 40, 2);
	hard_rustheap_enemy2.addReward(&hard_rustheap_enemy2_drop2, 20, 1);
	hard_rustheap_enemy2.addReward(&hard_rustheap_enemy2_drop3, 20, 1);
	hard_rustheap_enemy2.addReward(&hard_rustheap_enemy2_drop4, 15, 1);
	hard_rustheap_enemy2.addReward(&hard_rustheap_enemy2_drop5, 20, 1);
	hard_rustheap_enemy2.setSoulStoneDropChance(24);
	hard_rustheap_enemy2.setDescription("An elite gecko drummer who beats the earth itself. The rhythm is seismic. Those who hear it too long begin to feel it in their bones \x97 then feel nothing at all.");
	CombatLocation hard_rustheap_clocation2("Rustheap Upper Slopes, The Percussion Crater", &hard_rustheap_enemy2);
	hard_rustheap_locations.push_back(&hard_rustheap_clocation2);
	
	//Rustheap | Hard Enemy no. 3 | Voidscream
	// Fastest hard enemy (speed 9), always goes first. Glass cannon — lower HP compensated by relentless offense.
	// Shatter Chorus avg 10x2=20 dmg — a sustained high chain that can delete the player.
	// Subsonic Rupture DOT (-6/tick x4 = -24 HP) is the run-ending threat if both it and Shatter fire same fight.
	// Player must end this fast before Voidscream dictates the tempo.
	HpStatusEffect hard_rustheap_enemy3_status1("Subsonic Rupture", 4, 2, -6);
	StatusEffectSpell hard_rustheap_enemy3_spell1("Hemorrhage Scream", "A concentrated sonic blast aimed at the bloodstream, causing vessels to rupture from the inside.", 15, 4, 70, &hard_rustheap_enemy3_status1, "Sound");
	Spell hard_rustheap_enemy3_spell2("Wall Shatter", "A single scream at a precise frequency that shatters anything brittle — bone, metal, armor, will.", 18, 3, "Sound");
	ChainingSpell hard_rustheap_enemy3_spell3("Shatter Chorus", "The voice breaks into layered harmonics, each frequency targeting a different weakness, hitting in relentless succession.", 2, 5, 8, 12, "Sound");
	Spell* hard_rustheap_enemy3_spells[4] = {&hard_rustheap_enemy3_spell1, &hard_rustheap_enemy3_spell2, &hard_rustheap_enemy3_spell3, nullptr};
	Enemy hard_rustheap_enemy3("Voidscream", 72, 9, 8, 9, 20, 1.6, hard_rustheap_enemy3_spells);
	Consumable hard_rustheap_enemy3_drop1("Heap-Rot Raccoon Flesh", "Stringy raccoon meat soaked in the foul rust-stained filth of the lower heaps.", 10);
	Scroll hard_rustheap_enemy3_drop2("", &hard_rustheap_enemy3_spell1);
	Scroll hard_rustheap_enemy3_drop3("", &hard_rustheap_enemy3_spell2);
	Weapon hard_rustheap_enemy3_drop4("Voidscream's Resonance Amplifier", "Amplifier", "A neck-mounted resonance device that amplifies vocal output to weaponized levels. Still humming faintly.", 9);
	Armor hard_rustheap_enemy3_drop5("Soundproof Plated Hood", "A thick padded hood with internal lead lining. Reduces incoming sonic damage. Smells like fear.", 6, 1);
	hard_rustheap_enemy3.addReward(&hard_rustheap_enemy3_drop1, 40, 2);
	hard_rustheap_enemy3.addReward(&hard_rustheap_enemy3_drop2, 20, 1);
	hard_rustheap_enemy3.addReward(&hard_rustheap_enemy3_drop3, 20, 1);
	hard_rustheap_enemy3.addReward(&hard_rustheap_enemy3_drop4, 15, 1);
	hard_rustheap_enemy3.addReward(&hard_rustheap_enemy3_drop5, 20, 1);
	hard_rustheap_enemy3.setSoulStoneDropChance(25);
	hard_rustheap_enemy3.setDescription("An elite vocalist whose screams have been augmented beyond biological limits. The voice tears walls. Those caught in the wave are found with blood in their ears and no memory of the last few moments.");
	CombatLocation hard_rustheap_clocation3("Rustheap Upper Slopes, The Screaming Chamber", &hard_rustheap_enemy3);
	hard_rustheap_locations.push_back(&hard_rustheap_clocation3);
	
	// =============================================
	//  RUSTHEAP BOSS
	// =============================================
	
	//Rustheap | Boss | Scrapmonger, the Colossus of Rustheap
	// The final boss. 250 HP. No one knows if it was built or simply became.
	// Speed 4 = ties player. Every round is a trade — and Scrapmonger wins trades of attrition.
	//
	// Spell design:
	//   Heap Collapse (24, CD5)         — the spike. Rare but fight-defining.
	//   Debris Crush (18, CD3)          — steady 18 dmg every 3 rounds. Constant pressure.
	//   Tectonic Ram (15, CD2)          — fires constantly. 15 dmg/2 rounds = ~7.5 dmg/round sustained.
	//   Shrapnel Cyclone avg 16x1.5=24  — the most dangerous move. High chain, high mult, can end the fight.
	//
	// Sustained base dmg 12/round + constant spell cycling = player MUST be aggressive and use consumables.
	// DoT on Heap Collapse: Crushed Internals (-7/tick x4 = -28 HP) can combine with Cyclone for a lethal turn.
	// Soul Stone use is intended here — this is the payoff fight.
	HpStatusEffect rustheap_boss1_status1("Crushed Internals", 4, 2, -7);
	StatusEffectSpell rustheap_boss1_spell1("Heap Collapse", "Scrapmonger raises a colossal arm and brings the entire weight of the upper heap crashing down upon the target.", 24, 5, 70, &rustheap_boss1_status1, "Physical");
	Spell rustheap_boss1_spell2("Debris Crush", "A mass of compacted refuse is driven forward with the force of a collapsing wall.", 18, 3, "Physical");
	Spell rustheap_boss1_spell3("Tectonic Ram", "Scrapmonger lowers its body and charges — not fast, but with the mass of a collapsed structure behind every step.", 15, 2, "Physical");
	ChainingSpell rustheap_boss1_spell4("Shrapnel Cyclone", "The colossus spins, sending a storm of metal fragments outward in an unending lethal spiral.", 1.5, 4, 12, 18, "Metal");
	Spell* rustheap_boss1_spells[4] = {&rustheap_boss1_spell1, &rustheap_boss1_spell2, &rustheap_boss1_spell3, &rustheap_boss1_spell4};
	Enemy rustheap_boss1("Scrapmonger, the Colossus of Rustheap", 250, 12, 9, 4, 15, 1.5, rustheap_boss1_spells);
	Armor rustheap_boss1_drop1("Scrapmonger's Fused Hull Plate", "A chest-sized slab of fused metal pried from Scrapmonger's outer shell. Still warm. Still humming with something.", 14, 2);
	Armor rustheap_boss1_drop2("Scrapmonger's Compressed Gauntlets", "Gauntlets formed from condensed refuse, hardened under Scrapmonger's own weight over decades.", 7, 3);
	Weapon rustheap_boss1_drop3("Scrapmonger's Debris Arm", "Club", "A massive club-limb broken from the colossus's body. It weighs as much as a person. It swings like it knows.", 12);
	Scroll rustheap_boss1_drop4("", &rustheap_boss1_spell1);
	Scroll rustheap_boss1_drop5("", &rustheap_boss1_spell4);
	Consumable rustheap_boss1_drop6("Heap-Core Fragment", "A dense, pulsing chunk of compressed refuse from deep within Scrapmonger's body. Consuming it feels like swallowing the heap's history.", 30);
	rustheap_boss1.addReward(&rustheap_boss1_drop1, 60, 1);
	rustheap_boss1.addReward(&rustheap_boss1_drop2, 60, 1);
	rustheap_boss1.addReward(&rustheap_boss1_drop3, 40, 1);
	rustheap_boss1.addReward(&rustheap_boss1_drop4, 25, 1);
	rustheap_boss1.addReward(&rustheap_boss1_drop5, 25, 1);
	rustheap_boss1.addReward(&rustheap_boss1_drop6, 60, 3);
	rustheap_boss1.setSoulStoneDropChance(50);
	rustheap_boss1.setDescription("No one knows who built Scrapmonger, or if it was built at all \x97 some believe the heap simply accumulated enough refuse to begin thinking. It does not speak. It does not negotiate. It has no goal beyond reducing everything within reach to the same flattened ruin it already inhabits.");
	CombatLocation boss_rustheap_clocation1("The Heart of the Heap, Scrapmonger's Domain", &rustheap_boss1);
	vector<Location*> boss_rustheap_locations = {&boss_rustheap_clocation1};
	
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
	
	rustheap.generateMap(
		easy_rustheap_locations,
		medium_rustheap_locations,
		hard_rustheap_locations,
		boss_rustheap_locations,
		12, 10, 3
		);
	
	// Store maps in a vector (allows future expansion to multiple maps)
	vector<Map*> maps = { &snailsville, &rustheap };
	
	// Fast-forward to saved map and room (for new games = map 0, room 0)
	maps[currentMapIdx]->setPlayerPosition(currentMapIndex);
	
	// =============================================
	//  MAIN GAME LOOP
	// =============================================
	while (true) {
		Map* currentMap = maps[currentMapIdx];
		setMapColor(currentMap->getName());
		
		// Save before every room — position is the current room index
		{
			string label = currentMap->getName() + " - " + currentMap->getLocations()[currentMap->getPlayerPosition()]->getName();
			saveGame(currentMap->getPlayerPosition(), currentMapIdx, player, slot, label);
		}
		
		system("cls");
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		cout << player;
		cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
		
		currentMap->movePlayer();
		
		// ---- DEATH HANDLING ----
		if (player.getCurrentHp() <= 0) {
			resetColor();
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << "   The world goes dark around you..." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			
			if (player.useSoulStone()) {
				// Reload the save — which was written right before this room
				int savedRoom, savedMapIdx;
				loadGame(savedRoom, savedMapIdx, player, slot);
				currentMapIdx = savedMapIdx;
				currentMap->setPlayerPosition(savedRoom);
				
				// *** CRITICAL FIX: SAVE AFTER USING SOUL STONE ***
				{
					string label = currentMap->getName() + " - " + currentMap->getLocations()[savedRoom]->getName();
					saveGame(savedRoom, savedMapIdx, player, slot, label);
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
				resetColor();
				deleteSave(slot);
				system("pause");
				break;
			}
		}
		
		// ---- MAP COMPLETION ----
		if (currentMap->getPlayerFinished()) {
			currentMapIdx++;
			
			// All maps cleared -- true ending
			if (currentMapIdx >= (int)maps.size()) {
				resetColor();
				system("cls");
				cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				cout << " You have survived everything." << endl;
				cout << " " << player.getName() << " steps out of the dark for good." << endl;
				cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
				deleteSave(slot);
				system("pause");
				break;
			}
			
			// Advance to next map
			resetColor();
			system("cls");
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			cout << " " << currentMap->getName() << " falls behind you." << endl;
			cout << " The road ahead leads to " << maps[currentMapIdx]->getName() << "." << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
			maps[currentMapIdx]->setPlayerPosition(0);
			saveGame(0, currentMapIdx, player, slot, maps[currentMapIdx]->getName() + " - " + maps[currentMapIdx]->getLocations()[0]->getName());
			system("pause");
		}
	}
	
	return 0;
}
