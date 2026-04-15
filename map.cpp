#include "map.h"
#include "groupcombatlocation.h"
#include "heallocation.h"
#include "groupbattlemanager.h"
#include <iostream>
using namespace std;

// constructors
Map::Map() {
	this->name = "";
	this->playerPosition = 0;
	this->playerFinished = false;
}

Map::Map(string name, Player* player, vector<Location*> locations) {
	this->name = name;
	this->player = player;
	this->locations = locations;
	this->playerPosition = 0;
	this->playerFinished = false;
}

Map::Map(string name, Player* player) {
	this->name = name;
	this->player = player;
	this->playerPosition = 0;
	this->playerFinished = false;
}

// getters
string Map::getName() {
	return this->name;
}
Player* Map::getPlayer() {
	return this->player;
}
vector<Location*>& Map::getLocations() {
	return this->locations;
}
int Map::getPlayerPosition() {
	return this->playerPosition;
}
bool Map::getPlayerFinished() {
	return this->playerFinished;
}

// setters
void Map::setName(string name) {
	this->name = name;
}
void Map::setPlayer(Player* player) {
	this->player = player;
}
void Map::setLocations(vector<Location*> locations) {
	this->locations = locations;
}
void Map::setPlayerPosition(int playerPosition) {
	this->playerPosition = playerPosition;
}
void Map::setPlayerFinished(bool playerFinished) {
	this->playerFinished = playerFinished;
}

// working with the vector
void Map::addLocation(Location* location) {if (location != nullptr) this->locations.push_back(location);}

void Map::removeLocation(int index) {
	if (index < 0 || index >= locations.size()) return;
	delete locations[index];
	locations.erase(locations.begin() + index);
}

// typing out map
ostream& operator<<(ostream& output, Map m) {
	for (int i = 0; i < 3; i++) {
		for (int j = m.getPlayerPosition(); j < m.locations.size(); j++) {
			Location* loc = m.locations[j];
			
			if (loc != nullptr) {
				
				if (i == 0) {
					if (j == m.getPlayerPosition()) output << "++---++ ";
					else output << "+---+ ";
				}
				
				else if (i == 1) {
					if (j == m.getPlayerPosition()) output << "|| ";
					else output << "| ";
					
					if(dynamic_cast<GroupCombatLocation * >(loc) != nullptr || dynamic_cast<MultipleCombatLocation * >(loc) != nullptr || dynamic_cast<CombatLocation * >(loc) != nullptr) output << "C ";
					else if(dynamic_cast<HealLocation *>(loc) != nullptr) output << "H ";
					else if(dynamic_cast<TreasureLocation * >(loc) != nullptr) output << "T ";	
					else output << "  ";
					
					if (j == m.getPlayerPosition()) output << "|| ";
					else output << "| ";
				}
				
				else if (i == 2) {
					if (j == m.getPlayerPosition()) output << "++---++ ";
					else output << "+---+ ";
				}
			}
		}
		output << endl;
	}
	
	if (!m.locations.empty() && m.getPlayerPosition() < m.locations.size()) {
		output << m.name << " - "
		<< m.locations[m.getPlayerPosition()]->getName();
	}
	
	return output;
}

// moving player
void Map::movePlayer() {
	int playerChoice = 0;
	int inventoryChoice;
	if (playerPosition >= locations.size()) {
		checkPlayerFinished();
		return;
	}
	
	GroupCombatLocation* gcL = dynamic_cast<GroupCombatLocation*>(locations[playerPosition]);
	MultipleCombatLocation* mcL = dynamic_cast<MultipleCombatLocation*>(locations[playerPosition]);
	CombatLocation* cL = (gcL == nullptr && mcL == nullptr) ? dynamic_cast<CombatLocation*>(locations[playerPosition]) : nullptr;
	TreasureLocation* tL = dynamic_cast<TreasureLocation*>(locations[playerPosition]);
	HealLocation* hL = dynamic_cast<HealLocation*>(locations[playerPosition]);
	BattleManager b(this->player, nullptr);
	
	checkPlayerFinished();
	if (this->playerFinished) {
		system("cls");
		cout << "==========================================" << endl;
		cout << "You made it " + player->getName() + "..." << endl;
		cout << "The cursed lands of " + this->name + " lie behind you." << endl;
		cout << "For now." << endl;
		cout << "==========================================" << endl;
		return;
	}
	
	do {
		playerChoice = -1;
		system("cls");
		cout << *this << endl;
		cout << "==========================================" << endl;
		cout << "The path ahead whispers your name... what will you do?" << endl;
		cout << "1. Walk deeper into the darkness" << endl;
		cout << "2. Check your belongings" << endl;
		cout << "==========================================" << endl;
		cout << "What is your choice? " << endl;
		cin >> playerChoice;
		
		if (playerChoice == 2) {
			int inventoryChoice = -1;
			do {
				system("cls");
				
				cout << "==========================================" << endl;
				cout << *this->player;
				cout << "==========================================" << endl;
				cout << "You open your pack. The weight of your journey rests within." << endl;
				cout << "------------------------------------------" << endl;
				
				cout << this->player->getInventory() << endl;
				
				cout << "==========================================" << endl;
				cout << "0. Close your pack and return to the shadows." << endl;
				cout << "Choose an item to inspect... or disturb." << endl;
				cout << "==========================================" << endl;
				
				cin >> inventoryChoice;
				
				if (inventoryChoice == 0) {
					break;
				}
				
				vector<Item*>& items = this->player->getInventory().getStorage();
				
				if (inventoryChoice > 0 && inventoryChoice <= items.size()) {
					system("cls");
					
					Item* selectedItem = items[inventoryChoice - 1];
					if (selectedItem != nullptr) {
						Weapon* placeholderW = dynamic_cast<Weapon*>(items[inventoryChoice - 1]);
						Scroll* placeholderS = dynamic_cast<Scroll*>(items[inventoryChoice - 1]);
						Armor* placeholderA = dynamic_cast<Armor*>(items[inventoryChoice - 1]);
						Consumable* placeholderC = dynamic_cast<Consumable*>(items[inventoryChoice - 1]);
						AttackGadget* placeholderAG = dynamic_cast<AttackGadget*>(items[inventoryChoice - 1]);
						
						if (placeholderW != nullptr) {
							Weapon* playerW = player->getWeapon();
							Weapon* safe;
							int wantsToEquip = 0;
							
							cout << "==========================================" << endl;
							cout << "Your current weapon bears the scars of your journey." << endl;
							cout << *playerW << " (your trusted steel)" << endl;
							cout << "------------------------------------------" << endl;
							cout << "This one has yet to taste blood in your name." << endl;
							cout << *placeholderW << endl;
							cout << "==========================================" << endl;
							cout << "Only one may remain in your hand. Choose. (1 - Take it)" << endl;
							cin >> wantsToEquip;
							
							if (wantsToEquip == 1) {
								safe = playerW;
								
								player->setWeapon(placeholderW);
								
								items[inventoryChoice - 1] = safe;
								
								cout << "------------------------------------------" << endl;
								cout << "The old blade falls silent." << endl;
								cout << "The new one answers your will." << endl;
							}
							cout << "==========================================" << endl;
							system("pause");
						} else if (placeholderS != nullptr) {
							ChainingSpell* scrollCs = dynamic_cast<ChainingSpell*>(placeholderS->getHeldSpell());
							StatusEffectSpell* scrollSas = dynamic_cast<StatusEffectSpell*>(placeholderS->getHeldSpell());
							LifeStealSpell* scrollLss = dynamic_cast<LifeStealSpell*>(placeholderS->getHeldSpell());
							int wantsToLearn = 0;
							
							cout << "==========================================" << endl;
							cout << "The knowledge carved into your mind:" << endl << endl;
							for (int i = 0; i < 4; i++) {
								Spell *s = player->getSpells()[i];
								
								cout << " " << i + 1 << ". ";
								cout << "Etched into your soul: ";
								if (s == nullptr) {
									cout << "A hollow space within your mind." << endl << endl;
								} else {
									ChainingSpell* cs = dynamic_cast<ChainingSpell*>(player->getSpells()[i]);
									StatusEffectSpell* sas = dynamic_cast<StatusEffectSpell*>(player->getSpells()[i]);
									LifeStealSpell* lss = dynamic_cast<LifeStealSpell*>(player->getSpells()[i]);
									
									if (cs != nullptr) {
										cout << *cs;
									} else if (sas != nullptr) {
										cout << *sas;
									} else if (lss != nullptr) {
										cout << *lss;
									} else {
										cout << *s;
									}
								}
							}
							
							cout << "------------------------------------------" << endl;
							cout << "And the forbidden words bound within the scroll:" << endl << endl;
							if (scrollCs != nullptr) cout << *scrollCs;
							else if (scrollSas != nullptr) cout << *scrollSas;
							else if (scrollLss != nullptr) cout << *scrollLss;
							else cout << *placeholderS->getHeldSpell();
							cout << "==========================================" << endl;
							cout << "Adventure, this knowledge comes at a price. Your old spell will be lost." << endl;
							cout << "Select the spell you wish to sacrifice: ";
							cin >> wantsToLearn;
							
							if (wantsToLearn > 0 && wantsToLearn < 5) {
								player->setSpell(wantsToLearn - 1, placeholderS->getHeldSpell());
								
								cout << "------------------------------------------" << endl;
								cout << "A memory tears free from your mind." << endl;
								cout << "You can no longer recall its words." << endl;
								
								items.erase(items.begin() + (inventoryChoice - 1));
							}
							cout << "==========================================" << endl;
							system("pause");
						} else if (placeholderA != nullptr) {
							int wantsToEquip = 0;
							Armor* playerA;
							Armor* safe;
							
							switch (placeholderA->getType()) {
							case 1:
								//case where player is trying to equip helmet
								playerA = player->getHelmet();
								cout << "==========================================" << endl;
								if (player->getHelmet() == nullptr) cout << "Your crown rests bare, your spirit unguarded." << endl;
								else cout << "The helmet you bear hums softly against your mind..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new helm awaits, whispering as though it knows your soul:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may merge with your essence. Will you claim it? (1 - Bind it)" << endl;
								cin >> wantsToEquip;
								
								if (wantsToEquip == 1 && playerA != nullptr) {
									safe = playerA;
									
									player->setHelmet(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old helmet releases its hold with a final sigh." << endl;
									cout << "The new one melds with your spirit, its will now entwined with yours." << endl;
								} else if (wantsToEquip == 1 && playerA == nullptr) {
									player->setHelmet(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You place the helmet upon your head." << endl;
									cout << "It molds to your thoughts and becomes one with you." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 2:
								//case where player is trying to equip chestplate
								playerA = player->getChestplate();
								cout << "==========================================" << endl;
								if (player->getChestplate() == nullptr) cout << "Your chest feels naked, exposed to the void." << endl;
								else cout << "The chestplate you wear catches the light, tested in battle:" << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new chestplate awaits, ready to serve you:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one can claim your body and your will. Will you let it? (1 - Take it)" << endl;
								cin >> wantsToEquip;
								
								if (wantsToEquip == 1 && playerA != nullptr) {
									safe = playerA;
									
									player->setChestplate(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old chestplate slides away, its grip released." << endl;
									cout << "The new one settles into place, solid and sure." << endl;
								} else if (wantsToEquip == 1 && playerA == nullptr) {
									player->setChestplate(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You don the chestplate for the first time." << endl;
									cout << "It settles over your body, intertwining with your very essence." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 3:
								//case where player is trying to equip gloves
								playerA = player->getGloves();
								cout << "==========================================" << endl;
								if (player->getGloves() == nullptr) cout << "Your hands are bare, feeling the weight of the world." << endl;
								else cout << "The gloves you wear have served you well so far:" << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of gloves waits, ready for your hand:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your hands and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if (wantsToEquip == 1 && playerA != nullptr) {
									safe = playerA;
									
									player->setGloves(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old gloves fall away, releasing their hold." << endl;
									cout << "The new pair fits your hands well." << endl;
								} else if (wantsToEquip == 1 && playerA == nullptr) {
									player->setGloves(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You slip your hands into the gloves for the first time." << endl;
									cout << "They fit well, fingers moving freely within." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 4:
								//case where player is trying to equip leggings
								playerA = player->getLeggings();
								cout << "==========================================" << endl;
								if (player->getLeggings() == nullptr) cout << "Your legs move unguarded, every step unshielded." << endl;
								else cout << "The leggings you wear have carried you through many roads:" << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of leggings lies before you, ready to be worn:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your legs and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if (wantsToEquip == 1 && playerA != nullptr) {
									safe = playerA;
									
									player->setLeggings(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old leggings release their hold, falling away." << endl;
									cout << "The new pair fits well, sturdy around your legs." << endl;
								} else if (wantsToEquip == 1 && playerA == nullptr) {
									player->setLeggings(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You step into the leggings for the first time." << endl;
									cout << "They fit snugly, ready for the road ahead." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 5:
								//case where player is trying to equip boots
								playerA = player->getBoots();
								cout << "==========================================" << endl;
								if (player->getBoots() == nullptr) cout << "Your feet strike the ground unshielded, every step bare." << endl;
								else cout << "The boots you wear have kept pace with every step of your journey:" << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of boots waits, ready to carry you forward:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your feet and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if (wantsToEquip == 1 && playerA != nullptr) {
									safe = playerA;
									
									player->setBoots(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old boots loosen their hold and fall away." << endl;
									cout << "The new pair fits well, steady beneath your feet." << endl;
								} else if (wantsToEquip == 1 && playerA == nullptr) {
									player->setBoots(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You slide your feet into the boots for the first time." << endl;
									cout << "They fit well, solid and ready for the miles ahead." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							default:
								break;
							}
							
						} else if (placeholderC != nullptr) {
							int wantsToConsume = 0;
							
							cout << "==========================================" << endl;
							cout << *player;
							cout << "------------------------------------------" << endl;
							cout << "The item rests in your palm, worn from the road." << endl;
							cout << *placeholderC << endl;
							cout << "==========================================" << endl;
							cout << "Use it now? (1 - Use it)" << endl;
							cin >> wantsToConsume;
							
							if (wantsToConsume == 1) {
								player->setCurrentHp(player->getCurrentHp() + placeholderC->getHpToRecover());
								
								cout << "------------------------------------------" << endl;
								cout << "You consume it." << endl;
								cout << "Your wounds begin to close." << endl;
								cout << "You regain " << placeholderC->getHpToRecover() << " vitality." << endl;
								
								if (player->getMaxHp() <= player->getCurrentHp())
									cout << "You feel restored." << endl;
								
								items.erase(items.begin() + (inventoryChoice - 1));
							} else {
								cout << "You put it away." << endl;
							}
							
							cout << "==========================================" << endl;
							system("pause");
						} else if(placeholderAG != nullptr){
							cout << "==========================================" << endl;
							cout << *player;
							cout << "------------------------------------------" << endl;
							cout << "You turn the gadget over in your hands. It hums faintly — waiting." << endl;
							cout << *placeholderAG << endl;
							cout << "==========================================" << endl;
							cout << "This is not the place to use it. You tuck it back into the dark of your pack." << endl;
							system("pause");
						}
					}
				}
				
			} while (inventoryChoice != 0);
		}
	} while (playerChoice != 1);
	
	system("cls");
	
	if (gcL != nullptr) {
		GroupBattleManager gb(this->player, gcL->getEnemies());
		gb.battle();
		if (this->player->getCurrentHp() <= 0) return;
	} else if(mcL != nullptr){
		do{
			b = BattleManager(this->player, mcL->getLocations()[0]->getAttacker());
			b.battle();
			
			if (this->player->getCurrentHp() > 0) mcL->deleteLocation(0);
			
			// stop between phases if player died
			if (this->player->getCurrentHp() <= 0) return;
		} while(!mcL->getLocations().empty());
		
		// again, stop if player died
		if(this->player->getCurrentHp() <= 0) return;
	} else if (cL != nullptr) {
		b = BattleManager(this->player, cL->getAttacker());
		b.battle();
		
		// stop if player died
		if (this->player->getCurrentHp() <= 0) return;
	}
	else if(tL != nullptr){
		system("cls");
		cout << "==========================================" << endl;
		cout << " Before you sits " << tL->getTreasureName() << ", resting quietly at " << tL->getName() << "." << endl;
		cout << "------------------------------------------" << endl;
		cout << " 1. Pry it open" << endl;
		cout << " 2. Leave it be" << endl;
		cout << "------------------------------------------" << endl;
		cout << " What do you wish to do? ";
		int treasureChoice; cin >> treasureChoice;
		
		if (treasureChoice == 1) {
			// --- reward distribution (same logic as enemy loot) ---
			vector<Item*>& tItems   = tL->getRewardItems();
			vector<int>&   tChances = tL->getRewardChances();
			
			cout << "==========================================" << endl;
			cout << " You heave open " << tL->getTreasureName() << ". The lid groans and gives way..." << endl;
			cout << "------------------------------------------" << endl;
			
			// Soul Stone drop
			int ssChance = tL->getSoulStoneDropChance();
			if (ssChance > 0) {
				int ssRoll = rand() % 100 + 1;
				if (ssRoll <= ssChance) {
					if (this->player->getSoulStones() < 9) {
						this->player->addSoulStone();
						cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
						cout << " -> A cold glimmer rests at the very bottom." << endl;
						cout << " -> You reach in. A Soul Stone — humming faintly — settles in your palm." << endl;
						cout << " -> Soul Stones: ";
						for (int i = 0; i < this->player->getSoulStones(); i++) cout << "[*] ";
						cout << endl;
						cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
					} else {
						cout << " -> A Soul Stone glows inside, but your spirit is already full." << endl;
						cout << " -> It crumbles into ash as you touch it." << endl;
					}
				}
			}
			
			// Loot
			cout << " -> Within, you find:" << endl;
			bool anyDrop = false;
			for (size_t i = 0; i < tItems.size(); i++) {
				int roll = rand() % 100 + 1;
				if (roll <= tChances[i]) {
					cout << "     - " << tItems[i]->getName() << endl;
					this->player->getInventory().addItem(tItems[i]);
					anyDrop = true;
				}
			}
			if (!anyDrop) cout << "     - Nothing. Dust and old air — someone got here before you." << endl;
			
			// XP reward
			if (tL->getXpReward() > 0) {
				Player* p = dynamic_cast<Player*>(this->player);
				if (p != nullptr) {
					cout << "==========================================" << endl;
					p->setCurrentXp(p->getCurrentXp() + tL->getXpReward());
					cout << " -> The knowledge hidden within stirs something in you. You gain " << tL->getXpReward() << " XP." << endl;
				}
			}
			
			cout << "==========================================" << endl;
			system("pause");
		} else {
			cout << "==========================================" << endl;
			cout << " You turn away from " << tL->getTreasureName() << ", leaving it untouched." << endl;
			cout << " Perhaps that was wise. Perhaps not." << endl;
			cout << "==========================================" << endl;
			system("pause");
		}
	}
	else if(hL != nullptr){
		system("cls");
		cout << "==========================================" << endl;
		cout << " You arrived at " << hL->getName() << ", your well deserved quiet place." << endl;
		cout << " Before you stands " << hL->getHealName() << "." << endl;
		cout << "------------------------------------------" << endl;
		cout << " 1. Touch it" << endl;
		cout << " 2. Leave it be" << endl;
		cout << "------------------------------------------" << endl;
		cout << " What do you wish to do? ";
		int healChoice; cin >> healChoice;
		
		if(healChoice == 1){
			this->player->setCurrentHp(this->player->getMaxHp());
			
			cout << "==========================================" << endl;
			cout << " You lay your hand upon " << hL->getHealName() << "." << endl;
			cout << " You wounds heal, you feel as if you were reborn, your vitality is at max again" << endl;
			cout << "==========================================" << endl;
			system("pause");
		}
		else{
			cout << "==========================================" << endl;
			cout << " You turn away from " << hL->getHealName() << ", leaving it untouched." << endl;
			cout << " Your decision might have been wise, but I suppose it was not." << endl;
			cout << "==========================================" << endl;
			system("pause");
		}
	}
	
// move forward only if alive
	this->playerPosition += 1;
	
// check if finished
	checkPlayerFinished();
}

// check if player finished
void Map::checkPlayerFinished() {
	if (playerPosition >= locations.size()) {
		playerFinished = true;
	}
}

//function that generates the map
void Map::generateMap(vector<Location*>& easyLocations, vector<Location*>& mediumLocations, vector<Location*>& hardLocations, vector<Location*>& bossLocations, int easyCount, int mediumCount, int hardCount) {
	locations.clear(); // clear map
	this->playerPosition = 0;
	this->playerFinished = false;
	
	srand(time(NULL));
	
	//easy locations
	vector<Location*> easyRandom;
	if (!easyLocations.empty()) {
		vector<Location*> temp = easyLocations;
		for (int i = 0; i < temp.size(); i++) {
			int j = i + rand() % (temp.size() - i);
			swap(temp[i], temp[j]);
		}
		for (int i = 0; i < easyCount && i < temp.size(); i++) {
			easyRandom.push_back(temp[i]);
		}
	}
	locations.insert(locations.end(), easyRandom.begin(), easyRandom.end());
	
	//medium locations
	vector<Location*> mediumRandom;
	if (!mediumLocations.empty()) {
		vector<Location*> temp = mediumLocations;
		for (int i = 0; i < temp.size(); i++) {
			int j = i + rand() % (temp.size() - i);
			swap(temp[i], temp[j]);
		}
		for (int i = 0; i < mediumCount && i < temp.size(); i++) {
			mediumRandom.push_back(temp[i]);
		}
	}
	locations.insert(locations.end(), mediumRandom.begin(), mediumRandom.end());
	
	//hard locations
	vector<Location*> hardRandom;
	if (!hardLocations.empty()) {
		vector<Location*> temp = hardLocations;
		for (int i = 0; i < temp.size(); i++) {
			int j = i + rand() % (temp.size() - i);
			swap(temp[i], temp[j]);
		}
		for (int i = 0; i < hardCount && i < temp.size(); i++) {
			hardRandom.push_back(temp[i]);
		}
	}
	locations.insert(locations.end(), hardRandom.begin(), hardRandom.end());
	
	//boss location
	if (!bossLocations.empty()) {
		int idx = rand() % bossLocations.size();
		locations.push_back(bossLocations[idx]);
	}
	
	for (Location* loc : locations) {
		GroupCombatLocation* gcL2 = dynamic_cast<GroupCombatLocation*>(loc);
		if (gcL2 != nullptr) {
			for (Enemy* e : gcL2->getEnemies())
				if (e != nullptr) e->setCurrentHp(e->getMaxHp());
			continue;
		}
		CombatLocation* cL = dynamic_cast<CombatLocation*>(loc);
		if (cL != nullptr && cL->getAttacker() != nullptr) {
			cL->getAttacker()->setCurrentHp(cL->getAttacker()->getMaxHp());
		}
	}
	
	playerPosition = 0;
	playerFinished = false;
}
