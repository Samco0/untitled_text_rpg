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
	bool newGame = false;
	string playerName;
	
	//Player start up weapons and spells
	Spell sp1("Trembling Blow", "A weak, unsteady punch that barely fazes the target.", 3, 1);
	Spell sp2("Cursed Kick", "A timid kick, shadowed by a faint curse, barely affects the enemy.", 2.5, 1);
	Spell* s1[4] = { &sp1, &sp2, nullptr, nullptr};
	Weapon w1("Charred Wooden Tanto", "Sword", "A simple wooden tanto, slightly burnt and brittle, whispering faint dark secrets.", 1.5);
	
	//The player themselves
	Player player("", 25, 1.5, 0, 4, 20, 1.2, &w1, s1);
	
	//Start menu
	while (true) {
		cout << "===============================\n";
		cout << "       untitled text rpg    \n";
		cout << "===============================\n";
		cout << "1. Step into Shadows (New Game)\n";
		cout << "2. Recall Fallen (Load Game)\n";
		cout << "3. Burn Memories (Delete Save)\n";
		cout << "4. Fade Away (Exit)\n";
		cout << "Your choice: ";
		
		int choice;
		cin >> choice;
		
		if (choice == 1) { // New Game
			system("cls");
			cout << "Whisper your name to the void: ";
			cin >> playerName;
			player.setName(playerName);
			
			currentMapIndex = 0;
			newGame = true;
			// uložíme checkpoint na začátku první mapy
			saveGame(currentMapIndex, player, slot); 
			
			system("cls");
			cout << "Very well, " << player.getName() 
			<< ". You tread into the shadowed path, heart pounding and breath shallow...\n";
			break;
			
		} else if (choice == 2) { // Load Game
			cout << "The echoes of past adventurers linger:\n";
			for (int i = 1; i <= 3; i++) {
				ifstream f("slot" + to_string(i) + "_map.txt");
				cout << i << ". Grave " << i;
				if (f.is_open()) cout << " (contains traces)";
				cout << "\n";
			}
			
			cout << "Choose a grave to awaken (1-3), any other key to step back: ";
			int loadChoice;
			cin >> loadChoice;
			
			if (loadChoice >= 1 && loadChoice <= 3) {
				slot = loadChoice;
				if (loadGame(currentMapIndex, player, slot)) { // 🔥 load až teď
					cout << "Something stirs within Grave " << slot << "...\n";
					newGame = false;
					break;
				} else {
					cout << "Empty... nothing remains.\n";
				}
			} else {
				cout << "You retreat into the shadows...\n";
			}
		} else if (choice == 3) {
			cout << "Select grave to erase (1-3), any other key to cancel: ";
			int delChoice;
			cin >> delChoice;
			
			if (delChoice >= 1 && delChoice <= 3) {
				string mapFile = "slot" + to_string(delChoice) + "_map.txt";
				string itemsFile = "slot" + to_string(delChoice) + "_items.txt";
				
				bool deleted = false;
				if (fs::exists(mapFile)) { fs::remove(mapFile); deleted = true; }
				if (fs::exists(itemsFile)) { fs::remove(itemsFile); deleted = true; }
				
				if (deleted)
					cout << "Grave " << delChoice << " has been erased from memory.\n";
				else
					cout << "Nothing to erase in Grave " << delChoice << ".\n";
			} else {
				cout << "The urge passes. Returning...\n";
			}
			
		} else if (choice == 4) {
			cout << "You vanish into the void...\n";
			return 0;
			
		} else {
			cout << "No path for that choice. Try again.\n";
		}
		
		//Clear screen after iteration
		system("cls");
	}
	
	if (newGame) {
		saveGame(currentMapIndex, player, slot); //initial save
	}
	
	//Creating enemies and maps
	//Snailsville
	//Snailsville | Easy Enemy no.1 | Sputter the Smith
	Spell easy_snailsville_enemy1_spell1("Cracked Hammer", "The user shapes a jagged, darkened hammer from twisted metal and strikes the target's skull with a resonant clang.", 5, 3);
	Spell* easy_snailsville_enemy1_spells[4] = {&easy_snailsville_enemy1_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy1("Sputter the Smith", 12, 2, 1, 5, 15, 1.5, easy_snailsville_enemy1_spells);
	Consumable easy_snailsville_enemy1_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Scroll easy_snailsville_enemy1_drop2("Scroll of Cracked Hammer", &easy_snailsville_enemy1_spell1);
	Weapon easy_snailsville_enemy1_drop3("Blackened Shell Sword", "Sword", "Forged from shell and blackened iron, it hums faintly with a restless energy.", 2.5);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop1, 60, 3);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop2, 25, 1);
	easy_snailsville_enemy1.addReward(&easy_snailsville_enemy1_drop3, 15, 1);
	CombatLocation easy_snailsville_clocation1("The Snailsmith's Forge", &easy_snailsville_enemy1);
	vector<Location*> easy_snailsville_locations = {&easy_snailsville_clocation1};
	
//Snailsville | Easy Enemy no.2 | Shellcarver Apprentice
	Spell easy_snailsville_enemy2_spell1("Shattered Rock", "The user hurls a jagged fragment of stone, cracking it against the target with a sharp impact.", 7.5, 3);
	ChainingSpell easy_snailsville_enemy2_spell2("Pebble Barrage", "A swarm of sharp, unnatural pebbles erupts from the user's hands, cutting at the target relentlessly.", 1, 5, 4, 10);
	Spell* easy_snailsville_enemy2_spells[4] = {&easy_snailsville_enemy2_spell1, &easy_snailsville_enemy2_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy2("Shellcarver Apprentice", 14, 3, 2, 4, 15, 1.5, easy_snailsville_enemy2_spells);
	Consumable easy_snailsville_enemy2_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Scroll easy_snailsville_enemy2_drop2("Scroll of Shattered Rock", &easy_snailsville_enemy2_spell1);
	Scroll easy_snailsville_enemy2_drop3("Scroll of Pebble Barrage", &easy_snailsville_enemy2_spell2);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop1, 65, 3);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop2, 20, 1);
	easy_snailsville_enemy2.addReward(&easy_snailsville_enemy2_drop3, 15, 1);
	CombatLocation easy_snailsville_clocation2("A Pile of Rocks..?", &easy_snailsville_enemy2);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation2);
	
//Snailsville | Easy Enemy no.3 | Glimmering Courier
	Spell easy_snailsville_enemy3_spell1("Blinding Flash", "The user emits a sudden, piercing light that sears the target's eyes, leaving a lingering sting.", 4, 2);
	Spell* easy_snailsville_enemy3_spells[4] = {&easy_snailsville_enemy3_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy3("Glimmering Courier", 10, 1.5, 2, 7, 20, 2, easy_snailsville_enemy3_spells);
	Consumable easy_snailsville_enemy3_drop1("Glimmering Snail Flesh", "A chunk of snail flesh, glinting faintly as if it holds a strange, inner light.", 10);
	Scroll easy_snailsville_enemy3_drop2("Scroll of Blinding Flash", &easy_snailsville_enemy3_spell1);
	easy_snailsville_enemy3.addReward(&easy_snailsville_enemy3_drop1, 75, 3);
	easy_snailsville_enemy3.addReward(&easy_snailsville_enemy3_drop2, 25, 1);
	CombatLocation easy_snailsville_clocation3("The Lighthouse", &easy_snailsville_enemy3);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation3);
	
//Snailsville | Easy Enemy no.4 | Herbalist Snail
	Spell easy_snailsville_enemy4_spell1("Petal Maelstrom", "The user summons a spiraling storm of brittle, blackened petals that slice at anything in its path.", 5, 3);
	Spell easy_snailsville_enemy4_spell2("Thorn Lash", "The user lashes thorned vines at the target, tearing through flesh and armor alike.", 3.5, 2);
	Spell* easy_snailsville_enemy4_spells[4] = {&easy_snailsville_enemy4_spell1, &easy_snailsville_enemy4_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy4("Herbalist Snail", 10, 2, 1, 4, 10, 3, easy_snailsville_enemy4_spells);
	Consumable easy_snailsville_enemy4_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Consumable easy_snailsville_enemy4_drop2("Withered Rose", "A decayed, blackened rose oozing a faint, sticky residue.", 12.5);
	Scroll easy_snailsville_enemy4_drop3("Scroll of Petal Maelstrom", &easy_snailsville_enemy4_spell1);
	Weapon easy_snailsville_enemy4_drop4("Thorned Vine Whip", "Whip", "A whip braided from thorned vines, dark and twisted, leaving deep scratches.", 2);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop1, 50, 3);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop2, 30, 1);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop3, 15, 1);
	easy_snailsville_enemy4.addReward(&easy_snailsville_enemy4_drop4, 5, 1);
	CombatLocation easy_snailsville_clocation4("The Greenhouse", &easy_snailsville_enemy4);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation4);
	
//Snailsville | Easy Enemy no.5 | Lampbearer Snail
	Spell easy_snailsville_enemy5_spell1("Searing Gleam", "The user unleashes a piercing flash of corrupted light, scorching eyes and leaving a lingering daze.", 5.5, 3);
	ChainingSpell easy_snailsville_enemy5_spell2("Waxstorm", "The user trembles violently, raining molten wax shards that stick and burn, scalding flesh and armor alike.", 0.5, 5, 5, 20);
	Spell* easy_snailsville_enemy5_spells[4] = {&easy_snailsville_enemy5_spell1, &easy_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy5("Lampbearer Snail", 12.5, 2, 1, 4, 10, 1.25, easy_snailsville_enemy5_spells);
	Consumable easy_snailsville_enemy5_drop1("Snail Flesh", "A grisly chunk of snail, tough and faintly metallic.", 7.5);
	Weapon easy_snailsville_enemy5_drop2("Slimebound Lantern", "Lamp", "A warped lantern covered in snail slime; its flickering light dances unnaturally across the walls.", 1.5);
	Scroll easy_snailsville_enemy5_drop3("Scroll of Waxstorm", &easy_snailsville_enemy5_spell2);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop1, 60, 3);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop2, 30, 1);
	easy_snailsville_enemy5.addReward(&easy_snailsville_enemy5_drop3, 10, 1);
	CombatLocation easy_snailsville_clocation5("The Lampbearer's Lair", &easy_snailsville_enemy5);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation5);
	
//Snailsville | Easy Enemy no.6 | Tavern Worker Snail
	Spell easy_snailsville_enemy6_spell1("Barrel Smash", "The snail conjures a warped, oozing barrel and hurls it at full speed, splintering whatever it hits.", 10, 5);
	Spell easy_snailsville_enemy6_spell2("Piercing Brew", "The snail slaps its hands together, forming a condensed stream of fermented sludge that pierces the target.", 5, 3);
	Spell easy_snailsville_enemy6_spell3("Shattered Plate", "A jagged plate materializes in the snail's hand and flies like a deadly disc at the target.", 3, 2);
	Spell* easy_snailsville_enemy6_spells[4] = {&easy_snailsville_enemy6_spell1, &easy_snailsville_enemy6_spell2, &easy_snailsville_enemy6_spell3, nullptr};
	Enemy easy_snailsville_enemy6("Tavern Worker Snail", 15, 3, 3, 3, 7.5, 2, easy_snailsville_enemy6_spells);
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
	CombatLocation easy_snailsville_clocation6("The Rotting Tavern", &easy_snailsville_enemy6);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation6);
	
//Snailsville | Easy Enemy no.7 | Fisher Snail
	Spell easy_snailsville_enemy7_spell1("Mire Bubble", "The snail exhales a swollen, murky bubble of mucus that drifts forward before bursting violently against the target.", 5, 3);
	Spell* easy_snailsville_enemy7_spells[4] = {&easy_snailsville_enemy7_spell1, nullptr, nullptr, nullptr};
	Enemy easy_snailsville_enemy7("Fisher Snail", 10, 1.5, 1, 3, 15, 2, easy_snailsville_enemy7_spells);
	Consumable easy_snailsville_enemy7_drop1("Mirelake Catch", "A pale fish dragged from the depths of the mucus lake. Its skin feels wrong to the touch.", 7.5);
	Scroll easy_snailsville_enemy7_drop2("Scroll of Mire Bubble", &easy_snailsville_enemy7_spell1);
	Weapon easy_snailsville_enemy7_drop3("Mirebound Fishing Rod", "Fishing Rod", "A warped rod stiffened by dried slime and lake residue. It smells faintly of rot.", 1.75);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop1, 50, 3);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop2, 30, 1);
	easy_snailsville_enemy7.addReward(&easy_snailsville_enemy7_drop3, 20, 1);
	CombatLocation easy_snailsville_clocation7("The Mirelake Hut", &easy_snailsville_enemy7);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation7);
	
//Snailsville | Easy Enemy no.8 | Mud Leech
	LifeStealSpell easy_snailsville_enemy8_spell1("Sanguine Leech", "A writhing, vein-thick tendril lashes out and drains vitality straight from the victim's veins.", 10, 5, 80);
	Spell easy_snailsville_enemy8_spell2("Grave Mud Slam", "A hulking mass of grave-cold mud forms into a hand and crashes down upon the target.", 5, 3);
	Spell* easy_snailsville_enemy8_spells[4] = {&easy_snailsville_enemy8_spell1, &easy_snailsville_enemy8_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy8("Mud Leech", 12.5, 1.5, 2, 6, 15, 2, easy_snailsville_enemy8_spells);
	Consumable easy_snailsville_enemy8_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll easy_snailsville_enemy8_drop2("Scroll of Sanguine Leech", &easy_snailsville_enemy8_spell1);
	easy_snailsville_enemy8.addReward(&easy_snailsville_enemy8_drop1, 60, 3);
	easy_snailsville_enemy8.addReward(&easy_snailsville_enemy8_drop2, 40, 1);
	CombatLocation easy_snailsville_clocation8("The Drowned Hollow", &easy_snailsville_enemy8);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation8);
	
//Snailsville | Easy Enemy no.9 | Tiny Rotworm
	LifeStealSpell easy_snailsville_enemy9_spell1("Frail Siphon", "A thin tendril pierces the flesh and steals a whisper of vitality.", 5, 3, 60);
	Spell easy_snailsville_enemy9_spell2("Blight Spit", "The worm spits a string of foul, decaying bile at the target.", 3, 2);
	Spell* easy_snailsville_enemy9_spells[4] = {&easy_snailsville_enemy9_spell1, &easy_snailsville_enemy9_spell2, nullptr, nullptr};
	Enemy easy_snailsville_enemy9("Tiny Rotworm", 10, 2, 1, 4, 10, 3, easy_snailsville_enemy9_spells);
	Consumable easy_snailsville_enemy9_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll easy_snailsville_enemy9_drop2("Scroll of Frail Siphon", &easy_snailsville_enemy9_spell1);
	Scroll easy_snailsville_enemy9_drop3("Scroll of Blight Spit", &easy_snailsville_enemy9_spell2);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop1, 60, 3);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop2, 20, 1);
	easy_snailsville_enemy9.addReward(&easy_snailsville_enemy9_drop3, 20, 1);
	CombatLocation easy_snailsville_clocation9("The Rotpit Burrow", &easy_snailsville_enemy9);
	easy_snailsville_locations.push_back(&easy_snailsville_clocation9);
	
//Snailsville | Medium Enemy no.1 | Ironshell Knight
	Spell medium_snailsville_enemy1_spell1("Ferric Impact", "The knight encases his gauntlet in hardened iron and delivers a crushing blow.", 7.5, 3);
	Spell* medium_snailsville_enemy1_spells[4] = {&medium_snailsville_enemy1_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy1("Ironshell Knight", 20, 5, 3, 4, 10, 1.5, medium_snailsville_enemy1_spells);
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
	CombatLocation medium_snailsville_clocation1("The Ironbound Hall", &medium_snailsville_enemy1);
	vector<Location*> medium_snailsville_locations = {&medium_snailsville_clocation1};
	
//Snailsville | Medium Enemy no.2 | Shadow Sentry
	Spell medium_snailsville_enemy2_spell1("Umbral Kick", "A swift kick wreathed in condensed shadow energy.", 5, 2);
	Spell medium_snailsville_enemy2_spell2("Gloomblade", "A blade of hardened darkness forms and cleaves through the target.", 7.5, 3);
	ChainingSpell medium_snailsville_enemy2_spell3("Eclipse Barrage", "Weapons fall aside as the sentry's fists become void-shrouded, striking in relentless succession.", 1.25, 4, 1, 6);
	Spell* medium_snailsville_enemy2_spells[4] = {&medium_snailsville_enemy2_spell1, &medium_snailsville_enemy2_spell2, &medium_snailsville_enemy2_spell3, nullptr};
	Enemy medium_snailsville_enemy2("Shadow Sentry", 16, 5, 3, 7, 10, 2, medium_snailsville_enemy2_spells);
	Consumable medium_snailsville_enemy2_drop1("Condensed Shadow Flesh", "Cold, elastic meat infused with lingering darkness.", 15);
	Scroll medium_snailsville_enemy2_drop2("Scroll of Umbral Kick", &medium_snailsville_enemy2_spell1);
	Scroll medium_snailsville_enemy2_drop3("Scroll of Gloomblade", &medium_snailsville_enemy2_spell2);
	Scroll medium_snailsville_enemy2_drop4("Scroll of Eclipse Barrage", &medium_snailsville_enemy2_spell3);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop1, 55, 3);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop2, 15, 1);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop3, 15, 1);
	medium_snailsville_enemy2.addReward(&medium_snailsville_enemy2_drop4, 15, 1);
	CombatLocation medium_snailsville_clocation2("The Shaded Hollow", &medium_snailsville_enemy2);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation2);
	
	
//Snailsville | Medium Enemy no.3 | Leech Scout
	LifeStealSpell medium_snailsville_enemy3_spell1("Crimson Siphon Strike", "A brutal tendril-infused blow that tears vitality straight from the veins.", 8, 4, 85);
	Spell* medium_snailsville_enemy3_spells[4] = {&medium_snailsville_enemy3_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy3("Leech Scout", 15, 4, 3, 5, 15, 1.5, medium_snailsville_enemy3_spells);
	Consumable medium_snailsville_enemy3_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll medium_snailsville_enemy3_drop2("Scroll of Crimson Siphon Strike", &medium_snailsville_enemy3_spell1);
	medium_snailsville_enemy3.addReward(&medium_snailsville_enemy3_drop1, 75, 3);
	medium_snailsville_enemy3.addReward(&medium_snailsville_enemy3_drop2, 25, 1);
	CombatLocation medium_snailsville_clocation3("The Drowned Vein-Tunnel", &medium_snailsville_enemy3);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation3);
	
//Snailsville | Medium Enemy no.4 | Obsidian Shell Snail
	Spell medium_snailsville_enemy4_spell1("Shadowfist Strike", "The user envelops their claw in living shadow and slams it into the foe, chilling the soul", 7.5, 3);
	Spell* medium_snailsville_enemy4_spells[4] = {&medium_snailsville_enemy4_spell1, nullptr, nullptr, nullptr};
	Enemy medium_snailsville_enemy4("Obsidian Shell Snail", 20, 5, 3, 4, 10, 1.5, medium_snailsville_enemy4_spells);
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
	CombatLocation medium_snailsville_clocation4("Obsidianbound Hall of Whispers", &medium_snailsville_enemy4);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation4);
	
//Snailsville | Medium Enemy no.5 | Venomspire Snail
	HpStatusEffect medium_snailsville_enemy5_status1("Corrosive Venom", 5, 1, -1.5);
	StatusEffectSpell medium_snailsville_enemy5_spell1("Venomous Fang Strike", "A fang coated in a writhing black venom tears into the target, burning flesh and soul alike", 7.5, 4, 80, &medium_snailsville_enemy5_status1);
	HpStatusEffect medium_snailsville_enemy5_status2("Blight Poison", 3, 1, -1);
	StatusEffectSpell medium_snailsville_enemy5_spell2("Blighted Jab", "A quick strike that injects a creeping poison, leaving the target weakened and shivering", 5, 3, 80, &medium_snailsville_enemy5_status2);
	Spell* medium_snailsville_enemy5_spells[4] = {&medium_snailsville_enemy5_spell1, &medium_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy medium_snailsville_enemy5("Venomspire Snail", 19, 5, 3, 4, 12.5, 1.5, medium_snailsville_enemy5_spells);
	Consumable medium_snailsville_enemy5_drop1("Venomous Snail Meat", "The corpse of the snail radiates a dark, toxic essence; consuming it emboldens the daring and poisons the weak", 7.5);
	Scroll medium_snailsville_enemy5_drop2("Tome of Venomous Fang Strike", &medium_snailsville_enemy5_spell1);
	Scroll medium_snailsville_enemy5_drop3("Tome of Blighted Jab", &medium_snailsville_enemy5_spell2);
	Weapon medium_snailsville_enemy5_drop4("Shard of Venomspire Shell", "Knife", "A jagged shard from the snail's shell, still pulsing with dark energy", 2);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop1, 50, 3);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop2, 20, 1);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop3, 20, 1);
	medium_snailsville_enemy5.addReward(&medium_snailsville_enemy5_drop4, 10, 1);
	CombatLocation medium_snailsville_clocation5("Venomspire Tower of Dread", &medium_snailsville_enemy5);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation5);
	
//Snailsville | Medium Enemy no.6 | Creeping Mollusk
	Spell medium_snailsville_enemy6_spell1("Umbral Strike", "The user emerges from creeping darkness and delivers a sudden, silent blow", 7.5, 3);
	Spell medium_snailsville_enemy6_spell2("Shadowfall Kick", "The user bursts from the shadows and crashes into the target with a brutal kick", 10, 4);
	Spell* medium_snailsville_enemy6_spells[4] = {&medium_snailsville_enemy6_spell1, &medium_snailsville_enemy6_spell2, nullptr, nullptr};
	Enemy medium_snailsville_enemy6("Creeping Mollusk", 18, 5, 3, 7, 10, 2, medium_snailsville_enemy6_spells);
	Consumable medium_snailsville_enemy6_drop1("Cursed Mollusk Flesh", "A dark, rubbery slab of mollusk meat that carries a faint metallic taste and a whisper of corruption.", 7.5);
	Armor medium_snailsville_enemy6_drop2("Hood of the Silent Stalker", "A shadow-soaked hood once worn by a lurking predator of the night.", 2, 1);
	Armor medium_snailsville_enemy6_drop3("Robes of the Silent Stalker", "Tattered robes that swallow light, once belonging to a creature that hunted from darkness.", 3, 2);
	Weapon medium_snailsville_enemy6_drop4("Whispering Blade", "Knife", "A thin, sinister knife forged for silent kills in dark corners.", 2.5);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop1, 50, 3);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop2, 20, 1);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop3, 20, 1);
	medium_snailsville_enemy6.addReward(&medium_snailsville_enemy6_drop4, 10, 1);
	CombatLocation medium_snailsville_clocation6("Whispering Alley", &medium_snailsville_enemy6);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation6);
	
//Snailsville | Medium Enemy no.7 | Mudspike Leech
	ChainingSpell medium_snailsville_enemy7_spell1("Whirling Abyss Dance", "The user twists into a violent spinning assault, striking the target again and again with relentless force", 0.5, 4, 3, 20);
	LifeStealSpell medium_snailsville_enemy7_spell2("Thorn of the Bloodleecher", "The user launches a cursed thorn that burrows into the target and siphons their life essence", 7.5, 4, 90);
	ChainingSpell medium_snailsville_enemy7_spell3("Storm of Mire Spikes", "The user hurls a barrage of jagged mudspikes that rip through flesh like shards of stone", 1, 4, 3, 10);
	Spell* medium_snailsville_enemy7_spells[4] = {&medium_snailsville_enemy7_spell1, &medium_snailsville_enemy7_spell2, &medium_snailsville_enemy7_spell3, nullptr};
	Enemy medium_snailsville_enemy7("Mudspike Leech", 17, 6, 3, 5, 15, 1.25, medium_snailsville_enemy7_spells);
	Consumable medium_snailsville_enemy7_drop1("Blood-Slick Leech Meat", "Warm, rubbery flesh that still pulses faintly, as if reluctant to die.", 10);
	Scroll medium_snailsville_enemy7_drop2("Scroll of Whirling Abyss Dance", &medium_snailsville_enemy7_spell1);
	Scroll medium_snailsville_enemy7_drop3("Scroll of Thorn of the Bloodleecher", &medium_snailsville_enemy7_spell2);
	Scroll medium_snailsville_enemy7_drop4("Scroll of Storm of Mire Spikes", &medium_snailsville_enemy7_spell3);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop1, 50, 3);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop2, 20, 1);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop3, 20, 1);
	medium_snailsville_enemy7.addReward(&medium_snailsville_enemy7_drop4, 20, 1);
	CombatLocation medium_snailsville_clocation7("The Rotting Mire", &medium_snailsville_enemy7);
	medium_snailsville_locations.push_back(&medium_snailsville_clocation7);
	
//Snailsville | Hard Enemy no.1 | Bloodcarapace Marauder
	Spell hard_snailsville_enemy1_spell1("Iron Cataclysm Fist", "The marauder encases his arm in jagged metal and delivers a devastating strike.", 15, 4);
	HpStatusEffect hard_snailsville_enemy1_status1("Hemorrhage", 5, 2, -2);
	StatusEffectSpell hard_snailsville_enemy1_spell2("Carapace Rend", "Hardened claws tear deep, leaving a brutal bleeding wound.", 7.5, 2, 80, &hard_snailsville_enemy1_status1);
	LifeStealSpell hard_snailsville_enemy1_spell3("Crimson Devourer", "A massive tendril lashes out and drains vitality without mercy.", 10, 3, 100);
	Spell* hard_snailsville_enemy1_spells[4] = {&hard_snailsville_enemy1_spell1, &hard_snailsville_enemy1_spell2, &hard_snailsville_enemy1_spell3, nullptr};
	Enemy hard_snailsville_enemy1("Bloodcarapace Marauder", 35, 9, 5, 4, 10, 1.1, hard_snailsville_enemy1_spells);
	Scroll hard_snailsville_enemy1_drop1("Scroll of Carapace Rend", &hard_snailsville_enemy1_spell2);
	Scroll hard_snailsville_enemy1_drop2("Scroll of Crimson Devourer", &hard_snailsville_enemy1_spell3);
	Weapon hard_snailsville_enemy1_drop3("Bloodforged Carapace Mace", "Mace", "A brutal mace forged from dense snail metal.", 5);
	Armor hard_snailsville_enemy1_drop4("Crimson Carapace Cuirass", "A heavy chestplate forged from blood-hardened snail carapace, scarred by countless battles.", 7, 2);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop1, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop2, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop3, 25, 1);
	hard_snailsville_enemy1.addReward(&hard_snailsville_enemy1_drop4, 25, 1);
	CombatLocation hard_snailsville_clocation1("The Crimson Bastion", &hard_snailsville_enemy1);
	vector<Location*> hard_snailsville_locations = {&hard_snailsville_clocation1};
	
//Snailsville | Hard Enemy no.2 | Soulbound Enforcer
	Spell hard_snailsville_enemy2_spell1("Soulbreaker Fist", "A crushing strike infused with tormented soul energy.", 10, 3);
	ChainingSpell hard_snailsville_enemy2_spell2("Eternal Soulstorm", "Shattered fragments of fallen souls erupt in a relentless barrage.", 1, 5, 5, 15);
	Spell* hard_snailsville_enemy2_spells[4] = {&hard_snailsville_enemy2_spell1, &hard_snailsville_enemy2_spell2, nullptr, nullptr};
	Enemy hard_snailsville_enemy2("Soulbound Enforcer", 38, 7, 5, 5, 10, 1.5, hard_snailsville_enemy2_spells);
	Consumable hard_snailsville_enemy2_drop1("Fractured Soul Core", "A dim, whispering remnant of a warrior consumed by the Enforcer.", 20);
	Scroll hard_snailsville_enemy2_drop2("Scroll of Soulbreaker Fist", &hard_snailsville_enemy2_spell1);
	Armor hard_snailsville_enemy2_drop3("Soulbound Carapace Helm", "A dense snail-metal helm infused with restless spirits.", 4, 1);
	Armor hard_snailsville_enemy2_drop4("Soulbound Carapace Gauntlets", "Heavy gauntlets pulsing faintly with trapped souls.", 2.5, 3);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop1, 30, 3);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop2, 10, 1);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop3, 30, 1);
	hard_snailsville_enemy2.addReward(&hard_snailsville_enemy2_drop4, 30, 1);
	CombatLocation hard_snailsville_clocation2("The Soulbound Bastion", &hard_snailsville_enemy2);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation2);
	
//Snailsville | Hard Enemy no.3 | Venomspike Assassin
	HpStatusEffect hard_snailsville_enemy3_status1("Black Venom", 3, 6, -3);	
	StatusEffectSpell hard_snailsville_enemy3_spell1("Venomspike Shot", "The user launches a razor-sharp venomspike infused with deadly toxin", 12, 3, 80, &hard_snailsville_enemy3_status1);
	Spell hard_snailsville_enemy3_spell2("Umbral Jab", "The user melts into shadow, reappearing behind the target to drive a precise strike into their side", 8, 2);
	Spell hard_snailsville_enemy3_spell3("Umbral Kick", "The user emerges from the darkness and lands a brutal kick before fading back into shadow", 10, 3);
	Spell* hard_snailsville_enemy3_spells[4] = {&hard_snailsville_enemy3_spell1, &hard_snailsville_enemy3_spell2, &hard_snailsville_enemy3_spell3, nullptr};
	Enemy hard_snailsville_enemy3("Venomspike Assassin", 36, 8, 5, 6, 10, 1.5, hard_snailsville_enemy3_spells);
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
	CombatLocation hard_snailsville_clocation3("The Venomshroud Bastion", &hard_snailsville_enemy3);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation3);
	
//Snailsville | Hard Enemy no.4 | Deathcoil Snail
	Spell hard_snailsville_enemy4_spell1("Coiling Charge", "The snail coils into a spinning mass and violently slams into the target", 12, 3);
	ChainingSpell hard_snailsville_enemy4_spell2("Thunderous Spiral", "The creature spins uncontrollably, crashing into the target again and again like a living battering ram", 2, 3, 1, 6);
	Spell hard_snailsville_enemy4_spell3("Dust Vortex", "The snail spins at terrifying speed, unleashing a choking storm of dust and debris", 8, 2);
	Spell* hard_snailsville_enemy4_spells[4] = {&hard_snailsville_enemy4_spell1, &hard_snailsville_enemy4_spell2, &hard_snailsville_enemy4_spell3, nullptr};
	Enemy hard_snailsville_enemy4("Deathcoil Snail", 34, 9, 5, 7, 10, 1.5, hard_snailsville_enemy4_spells);
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
	CombatLocation hard_snailsville_clocation4("The Rolling Bastion", &hard_snailsville_enemy4);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation4);
	
//Snailsville | Hard Enemy no.5 | Obsidian Champion
	Spell hard_snailsville_enemy5_spell1("Obsidian Fist", "The champion cloaks its fist in jagged obsidian and strikes with crushing force", 16, 4);
	ChainingSpell hard_snailsville_enemy5_spell2("Shards of Obsidian", "The champion hurls razor-sharp obsidian shards in a deadly scatter towards the target", 1, 4, 1, 16);
	Spell* hard_snailsville_enemy5_spells[4] = {&hard_snailsville_enemy5_spell1, &hard_snailsville_enemy5_spell2, nullptr, nullptr};
	Enemy hard_snailsville_enemy5("Obsidian Champion", 40, 8, 5, 3, 10, 1.25, hard_snailsville_enemy5_spells);
	Armor hard_snailsville_enemy5_drop1("Obsidian Carapace", "A chestplate formed from dark, tempered obsidian", 7.5, 2);
	Armor hard_snailsville_enemy5_drop2("Obsidian Gauntlets", "Gauntlets crafted from razor-sharp obsidian shards", 3, 3);
	Scroll hard_snailsville_enemy5_drop3("Scroll of Obsidian Fist", &hard_snailsville_enemy5_spell1);
	Scroll hard_snailsville_enemy5_drop4("Scroll of Shards of Obsidian", &hard_snailsville_enemy5_spell2);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop1, 25, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop2, 25, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop3, 10, 1);
	hard_snailsville_enemy5.addReward(&hard_snailsville_enemy5_drop4, 10, 1);
	CombatLocation hard_snailsville_clocation5("The Obsidian Bastion", &hard_snailsville_enemy5);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation5);
	
// Snailsville | Hard Enemy no.6 | Tidal Leech Commander
	LifeStealSpell hard_snailsville_enemy6_spell1("Hydro Drain", "The commander splashes corrosive water, masking tendrils that sap vitality from the target", 16, 4, 80);
	HpStatusEffect hard_snailsville_enemy6_status1("Hemorrhage", 1, 5, -2);
	StatusEffectSpell hard_snailsville_enemy6_spell2("Water Spike", "A jet of water pierces the target, causing bleeding", 8, 3, 80, &hard_snailsville_enemy6_status1);
	ChainingSpell hard_snailsville_enemy6_spell3("Crimson Bubble Barrage", "The commander fires a relentless stream of blood-filled bubbles at the target", 0.5, 3, 1, 24);
	Spell* hard_snailsville_enemy6_spells[4] = {&hard_snailsville_enemy6_spell1, &hard_snailsville_enemy6_spell2, &hard_snailsville_enemy6_spell3, nullptr};
	Enemy hard_snailsville_enemy6("Tidal Leech Commander", 38, 8, 5, 4, 10, 1.5, hard_snailsville_enemy6_spells);
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
	CombatLocation hard_snailsville_clocation6("The Coral Bastion", &hard_snailsville_enemy6);
	hard_snailsville_locations.push_back(&hard_snailsville_clocation6);
	
//Snailsville | Boss no.1 | Claris, the Shell Empress
	HpStatusEffect snailsville_boss1_status1("Corrosive Venom", 5, 2, -3);
	StatusEffectSpell snailsville_boss1_spell1("Ooze Lash", "The user lashes a writhing mass of blackened slime at the target, corroding flesh and spirit.", 15, 3, 80, &snailsville_boss1_status1);
	Spell snailsville_boss1_spell2("Carapace Slam", "The user hurls their jagged shell at the target, crushing with brutal force.", 10, 2);
	StatusEffectSpell snailsville_boss1_spell3("Putrescent Cloud", "The user exhales a toxic cloud towards the target, sapping vitality.", 5, 2, 80, &snailsville_boss1_status1);
	Spell snailsville_boss1_spell4("Spinning Devastation", "The user spins in their shell at full speed, battering the target mercilessly.", 20, 5);
	Spell* snailsville_boss1_spells[4] = {&snailsville_boss1_spell1, &snailsville_boss1_spell2, &snailsville_boss1_spell3, &snailsville_boss1_spell4};
	Enemy snailsville_boss1("Claris, the Shell Empress", 80, 10, 7, 5, 5, 1.5, snailsville_boss1_spells);
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
	CombatLocation boss_snailsville_clocation1("The Blighted Citadel", &snailsville_boss1);
	vector<Location*> boss_snailsville_locations = {&boss_snailsville_clocation1};
	
//Snailsville | Boss no.2 | Mathan, the Bloodcarapace
	LifeStealSpell snailsville_boss2_spell1("Sanguine Sap", "The user rends the target's flesh with a cursed cut and siphons their life force.", 10, 3, 75);
	LifeStealSpell snailsville_boss2_spell2("Crimson Leeching", "The user bites deep, draining the vitality of the target through dark blood magic.", 10, 3, 90);
	Spell snailsville_boss2_spell3("Piercing Hemorrhage", "The user slams their hands together, unleashing a pressurized jet of coagulated blood at the target.", 12.5, 2);
	HpStatusEffect snailsville_boss2_status1("Hemorrhaging", 8, 3, 3);
	StatusEffectSpell snailsville_boss2_spell4("Bloody Web", "The user conjures a crimson web of congealed blood, slicing through the target.", 5, 3, 80, &snailsville_boss2_status1);
	Spell* snailsville_boss2_spells[4] = {&snailsville_boss2_spell1, &snailsville_boss2_spell2, &snailsville_boss2_spell3, &snailsville_boss2_spell4};
	Enemy snailsville_boss2("Mathian, the Bloodcarapace", 90, 8, 7, 6, 10, 1.5, snailsville_boss2_spells);
	Armor snailsville_boss2_drop1("Mathian's Carapace of Hemogore", "A chestplate molded from the hardened blood of countless victims, pulsating with dark vitality.", 8, 2);
	Armor snailsville_boss2_drop2("Mathian's Veinbound Greaves", "Leggings crafted from interlaced veins and coagulated blood, unnervingly alive.", 5, 4);
	Weapon snailsville_boss2_drop3("Mathian's Crimson Reaver", "Sword", "A blood-forged blade saturated with the essence of the slain.", 6);
	Scroll snailsville_boss2_drop4("Scroll of Sanguine Sap", &snailsville_boss2_spell1);
	Scroll snailsville_boss2_drop5("Scroll of Crimson Leeching", &snailsville_boss2_spell2);
	Scroll snailsville_boss2_drop6("Scroll of Piercing Hemorrhage", &snailsville_boss2_spell3);
	Scroll snailsville_boss2_drop7("Scroll of Bloody Web", &snailsville_boss2_spell4);
	Consumable snailsville_boss2_drop8("Royal Leechflesh", "Rubbery flesh from a royal leech, still warm with the blood it once consumed.", 25);
	snailsville_boss2.addReward(&snailsville_boss2_drop1, 50, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop2, 50, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop3, 35, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop4, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop5, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop6, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop7, 20, 1);
	snailsville_boss2.addReward(&snailsville_boss2_drop8, 50, 3);
	CombatLocation boss_snailsville_clocation2("The Crimson Bastion", &snailsville_boss2);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation2);
	
//Snailsville | Boss no.3 | Andreas, the Tidal Snail
	Spell snailsville_boss3_spell1("Tidal Crush", "The user summons a towering wave that smashes down upon the target.", 20, 5);
	ChainingSpell snailsville_boss3_spell2("Relentless Drizzle", "The user conjures a stormcloud, raining dagger-like drops upon the target", 0.5, 4, 10, 30);
	HpStatusEffect snailsville_boss3_status1("Drowning", 6, 1, 5 );
	StatusEffectSpell snailsville_boss3_spell3("Choking Bubble", "The user traps the target in a suffocating bubble", 5, 3, 75, &snailsville_boss3_status1);
	Spell snailsville_boss3_spell4("Maelstrom Spin", "The user creates a violent whirlpool that drags the target in", 10, 3);
	Spell* snailsville_boss3_spells[4] = {&snailsville_boss3_spell1, &snailsville_boss3_spell2, &snailsville_boss3_spell3, &snailsville_boss3_spell4};
	Enemy snailsville_boss3("Andreas, the Tidal Snail", 100, 12, 7, 3, 5, 1.5, snailsville_boss3_spells);
	Armor snailsville_boss3_drop1("Andreas's Carapace of Sunken Bones", "Chestplate forged from the bones of creatures lost to the abyss", 8.5, 2);
	Weapon snailsville_boss3_drop2("Andreas's Spear of Drowned Monarchs", "Spear", "A long, coral-encrusted spear once wielded by kings swallowed by the deep", 6);
	Scroll snailsville_boss3_drop3("Scroll of Tidal Crush", &snailsville_boss3_spell1);
	Scroll snailsville_boss3_drop4("Scroll of Relentless Drizzle", &snailsville_boss3_spell2);
	Scroll snailsville_boss3_drop5("Scroll of Choking Bubble", &snailsville_boss3_spell3);
	Scroll snailsville_boss3_drop6("Scroll of Maelstrom Spin", &snailsville_boss3_spell4);
	Consumable snailsville_boss3_drop7("Royal Abyssal Snail Meat", "Cold, rubbery flesh taken from a royal abyssal snail, still smelling of the deep sea.", 25);
	snailsville_boss3.addReward(&snailsville_boss3_drop1, 50, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop2, 35, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop3, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop4, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop5, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop6, 20, 1);
	snailsville_boss3.addReward(&snailsville_boss3_drop7, 50, 3);
	CombatLocation boss_snailsville_clocation3("The Sunken Spire", &snailsville_boss3);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation3);
	
//Snailsville | Boss no.4 | Peklorex, the Infernal Slug
	Spell snailsville_boss4_spell1("Searing Fist", "The user heats up their fist and crushes the target with molten force", 15, 4);
	HpStatusEffect snailsville_boss4_status1("Cursed Flames", 4, 1, 5);
	StatusEffectSpell snailsville_boss4_spell2("Infernal Breath", "The user exhales a scorching inferno upon the target", 7.5, 4, 65, &snailsville_boss4_status1);
	HpStatusEffect snailsville_boss4_status2("Boiling Veins", 3, 1, 7.5);
	StatusEffectSpell snailsville_boss4_spell3("Hellfire Heat", "The user burns the air around the target with demonic fire", 5, 5, 40, &snailsville_boss4_status2);
	ChainingSpell snailsville_boss4_spell4("Dance of Damnation", "The user writhes possessed by dark flames, striking with ferocity", 2.5, 4, 2, 6);
	Spell* snailsville_boss4_spells[4] = {&snailsville_boss4_spell1, &snailsville_boss4_spell2, &snailsville_boss4_spell3, &snailsville_boss4_spell4};
	Enemy snailsville_boss4("Peklorex, the Infernal Slug Tyrant", 110, 13, 8, 3, 5, 1.75, snailsville_boss4_spells);
	Armor snailsville_boss4_drop1("Peklorex's Obsidian Carapace", "A chestplate forged from cursed snail obsidian, worn by the tyrant", 10, 2);
	Armor snailsville_boss4_drop2("Peklorex's Gauntlets of Torment", "Gauntlets infused with the essence of fallen victims", 3.5, 3);
	Weapon snailsville_boss4_drop3("Peklorex's Soulfire War Axe", "War Axe", "A war axe of snailsidian, carrying the souls of the condemned", 6.5);
	Scroll snailsville_boss4_drop4("Scroll of Searing Fist", &snailsville_boss4_spell1);
	Scroll snailsville_boss4_drop5("Scroll of Infernal Breath", &snailsville_boss4_spell2);
	Scroll snailsville_boss4_drop6("Scroll of Hellfire Heat", &snailsville_boss4_spell3);
	Scroll snailsville_boss4_drop7("Scroll of Dance of Damnation", &snailsville_boss4_spell4);
	Consumable snailsville_boss4_drop8("Royal Ember Snail Meat", "Smoldering flesh from a royal heat-dwelling snail, hot enough to burn the unwary.", 25);
	snailsville_boss4.addReward(&snailsville_boss4_drop1, 50, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop2, 50, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop3, 35, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop4, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop5, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop6, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop7, 20, 1);
	snailsville_boss4.addReward(&snailsville_boss4_drop8, 50, 3);
	CombatLocation boss_snailsville_clocation4("The Sunken Spire of Torment", &snailsville_boss4);
	boss_snailsville_locations.push_back(&boss_snailsville_clocation4);
	
	// MAP INITIALIZATION
	
	// Create map instance and pass player reference to it
	// The map interacts directly with the player object (combat, movement, etc.)
	Map snailsville("The Snailsville", &player);
	
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
	
	
	// Load saved progress (player stats + map index)
	loadGame(currentMapIndex, player, slot);
	
	
	// MAIN GAME LOOP
	while (true) {
		// Get currently active map
		Map* currentMap = maps[currentMapIndex];
		
		// Save checkpoint before each turn/movement
		saveGame(currentMapIndex, player, slot);
		
		system("cls");
		
		// Display basic UI information
		cout << "==============================" << endl;
		cout << "Current map: " << currentMapIndex << endl;
		cout << "HP: " << player.getCurrentHp()
		<< "/" << player.getMaxHp() << endl;
		cout << "==============================" << endl;
		
		// Handle player movement (may trigger combat or events)
		currentMap->movePlayer();
		
		// PLAYER DEATH HANDLING
		// If player HP drops to 0 or below
		if (player.getCurrentHp() <= 0) {
			
			cout << "\nYou died. Reloading checkpoint...\n";
			
			// Attempt to reload last saved state
			if (loadGame(currentMapIndex, player, slot)) {
				cout << "Checkpoint loaded successfully.\n";
				system("pause");
				continue;  // Continue game loop after loading
			}
			else {
				// If no save exists, end the game
				cout << "No save found. Game over.\n";
				break;
			}
		}
		
		// MAP COMPLETION CHECK
		// If player finished current map
		if (currentMap->getPlayerFinished()) {
			
			cout << "Map cleared!\n";
			
			// Move to next map
			currentMapIndex++;
			
			// If no more maps exist → game completed
			if (currentMapIndex >= maps.size()) {
				cout << "You completed all maps!\n";
				break;
			}
		}
	}
	
	return 0;
}
