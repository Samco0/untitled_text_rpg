#include "map.h"
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

// getters
string Map::getName() { return this->name; }
Player* Map::getPlayer() { return this->player; }
vector<Location*>& Map::getLocations() { return this->locations; }
int Map::getPlayerPosition() { return this->playerPosition; }
bool Map::getPlayerFinished() { return this->playerFinished; }

// setters
void Map::setName(string name) { this->name = name; }
void Map::setPlayer(Player* player) { this->player = player; }
void Map::setLocations(vector<Location*> locations) { this->locations = locations; }
void Map::setPlayerPosition(int playerPosition) { this->playerPosition = playerPosition; }
void Map::setPlayerFinished(bool playerFinished) { this->playerFinished = playerFinished; }

// working with the vector
void Map::addLocation(Location* location) {
	if(location != nullptr)
		this->locations.push_back(location);
}

void Map::removeLocation(int index) {
	if(index < 0 || index >= locations.size()) return;
	delete locations[index];
	locations.erase(locations.begin() + index);
}

// typing out map
ostream& operator<<(ostream& output, Map m) {
	for(int i = 0; i < 3; i++){
		for(int j = m.getPlayerPosition(); j < m.locations.size(); j++){
			Location* loc = m.locations[j];
			
			if(loc != nullptr){
				
				if(i == 0){
					if(j == m.getPlayerPosition()){
						output << "++---++ ";
					} else {
						output << "+---+ ";
					}
				}
				
				else if(i == 1){
					if(j == m.getPlayerPosition()){
						output << "|| ";
					} else {
						output << "| ";
					}
					
					if(dynamic_cast<CombatLocation*>(loc) != nullptr){
						output << "C ";
					} else {
						output << "  ";
					}
					
					if(j == m.getPlayerPosition()){
						output << "|| ";
					} else {
						output << "| ";
					}
				}
				
				else if(i == 2){
					if(j == m.getPlayerPosition()){
						output << "++---++ ";
					} else {
						output << "+---+ ";
					}
				}
			}
		}
		output << endl;
	}
	
	if(!m.locations.empty() && m.getPlayerPosition() < m.locations.size()){
		output << m.name << " - "
		<< m.locations[m.getPlayerPosition()]->getName();
	}
	
	return output;
}

// moving player
void Map::movePlayer(){
	int playerChoice = 0;
	int inventoryChoice;
	if(playerPosition >= locations.size()) {
		checkPlayerFinished();
		return;
	}
	
	CombatLocation* cL = dynamic_cast<CombatLocation*>(locations[playerPosition]);
	BattleManager b(this->player, nullptr);
	
	if(cL != nullptr){
		b = BattleManager(this->player, cL->getAttacker());
	}
	
	checkPlayerFinished();
	if(this->playerFinished){
		system("cls");
		cout << "==========================================" << endl;
		cout << "You made it " + player->getName() + "..." << endl;
		cout << "The cursed lands of " + this->name + " lie behind you." << endl;
		cout << "For now." << endl;
		cout << "==========================================" << endl;
		return;
	}
	
	do{
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
		
		if(playerChoice == 2){
			int inventoryChoice = -1;
			do{
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
				
				if(inventoryChoice == 0){
					break;
				}
				
				vector<Item*>& items = this->player->getInventory().getStorage();
				
				if(inventoryChoice > 0 && inventoryChoice <= items.size()){
					system("cls");
					
					Item* selectedItem = items[inventoryChoice - 1];
					if(selectedItem != nullptr){
						Weapon* placeholderW = dynamic_cast<Weapon*>(items[inventoryChoice - 1]);
						Scroll* placeholderS = dynamic_cast<Scroll*>(items[inventoryChoice - 1]);
						Armor* placeholderA = dynamic_cast<Armor*>(items[inventoryChoice - 1]);
						Consumable* placeholderC = dynamic_cast<Consumable*>(items[inventoryChoice - 1]);
						
						if(placeholderW != nullptr){
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
							
							if(wantsToEquip == 1){
								safe = playerW;
								
								player->setWeapon(placeholderW);
								
								items[inventoryChoice - 1] = safe;
								
								cout << "------------------------------------------" << endl;
								cout << "The old blade falls silent." << endl;
								cout << "The new one answers your will." << endl;
							}
							cout << "==========================================" << endl;
							system("pause");
						}
						else if(placeholderS != nullptr){
							ChainingSpell* scrollCs = dynamic_cast<ChainingSpell*>(placeholderS->getHeldSpell());
							StatusEffectSpell* scrollSas = dynamic_cast<StatusEffectSpell*>(placeholderS->getHeldSpell());
							int wantsToLearn = 0;
							
							cout << "==========================================" << endl;
							cout << "The knowledge carved into your mind:" << endl << endl;
							for(int i = 0; i < 4; i++) {
								Spell *s = player->getSpells()[i];
								
								cout << " " << i+1 << ". ";
								cout << "Etched into your soul: ";
								if(s == nullptr) { 
									cout << "A hollow space within your mind." << endl << endl;
								} else {
									ChainingSpell* cs = dynamic_cast<ChainingSpell*>(player->getSpells()[i]);
									StatusEffectSpell* sas = dynamic_cast<StatusEffectSpell*>(player->getSpells()[i]);
									
									if(cs != nullptr) {
										cout << *cs;
									} else if(sas != nullptr) {
										cout << *sas;
									} else {
										cout << *s;
									}
								}
							}
							
							cout << "------------------------------------------" << endl;
							cout << "And the forbidden words bound within the scroll:" << endl << endl;
							if(scrollCs != nullptr) cout << *scrollCs;
							else if(scrollSas != nullptr) cout << *scrollSas;
							else cout << *placeholderS->getHeldSpell();
							cout << "==========================================" << endl;
							cout << "Adventure, this knowledge comes at a price. Your old spell will be lost." << endl;
							cout << "Select the spell you wish to sacrifice: ";
							cin >> wantsToLearn;
							
							if(wantsToLearn > 0 && wantsToLearn < 5){
								player->setSpell(wantsToLearn-1, placeholderS->getHeldSpell());
								
								cout << "------------------------------------------" << endl;
								cout << "A memory tears free from your mind." << endl;
								cout << "You can no longer recall its words." << endl;
								
								items.erase(items.begin() + (inventoryChoice - 1));
							}
							cout << "==========================================" << endl;
							system("pause");
						}
						else if(placeholderA != nullptr){
							int wantsToEquip = 0;
							Armor* playerA;
							Armor* safe;
							
							switch (placeholderA->getType()) {
							case 1:
								//case where player is trying to equip helmet
								playerA = player->getHelmet();
								cout << "==========================================" << endl;
								if(player->getHelmet() == nullptr) cout << "Your crown rests bare, your spirit unguarded." << endl;
								else cout << "The helmet you bear hums softly against your mind..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new helm awaits, whispering as though it knows your soul:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may merge with your essence. Will you claim it? (1 - Bind it)" << endl;
								cin >> wantsToEquip;
								
								if(wantsToEquip == 1 && playerA != nullptr){
									safe = playerA;
									
									player->setHelmet(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old helmet releases its hold with a final sigh." << endl;
									cout << "The new one melds with your spirit, its will now entwined with yours." << endl;
								}
								else if(wantsToEquip == 1 && playerA == nullptr){
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
								if(player->getChestplate() == nullptr) cout << "Your chest feels naked, exposed to the void." << endl;
								else cout << "The chestplate you wear hums against your flesh, aware of you..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new chestplate waits, pulsing like it knows your heartbeat:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one can claim your body and your will. Will you let it? (1 - Take it)" << endl;
								cin >> wantsToEquip;
								
								if(wantsToEquip == 1 && playerA != nullptr){
									safe = playerA;
									
									player->setChestplate(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old chestplate slides away, its grip released." << endl;
									cout << "The new one melds with your form, its presence now part of you." << endl;
								}
								else if(wantsToEquip == 1 && playerA == nullptr){
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
								if(player->getGloves() == nullptr) cout << "Your hands are bare, feeling the weight of the world." << endl;
								else cout << "The gloves you wear hum against your skin, sensing your touch..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of gloves waits, as if alive with your intent:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your hands and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if(wantsToEquip == 1 && playerA != nullptr){
									safe = playerA;
									
									player->setGloves(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old gloves fall away, releasing their hold." << endl;
									cout << "The new pair molds to your hands, ready to obey your command." << endl;
								}
								else if(wantsToEquip == 1 && playerA == nullptr){
									player->setGloves(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You slip your hands into the gloves for the first time." << endl;
									cout << "They cling to your fingers, as if they were made to move with your very will." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 4:
								//case where player is trying to equip leggings
								playerA = player->getLeggings();
								cout << "==========================================" << endl;
								if(player->getLeggings() == nullptr) cout << "Your legs move unguarded, every step unshielded." << endl;
								else cout << "The leggings you wear shift with you, sensing every motion..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of leggings lies before you, eager to meld with your form:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your legs and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if(wantsToEquip == 1 && playerA != nullptr){
									safe = playerA;
									
									player->setLeggings(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old leggings release their hold, falling away." << endl;
									cout << "The new pair melds with your legs, moving as one with your body." << endl;
								}
								else if(wantsToEquip == 1 && playerA == nullptr){
									player->setLeggings(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You step into the leggings for the first time." << endl;
									cout << "They wrap around your legs, moving as if part of your own body." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							case 5:
								//case where player is trying to equip boots
								playerA = player->getBoots();
								cout << "==========================================" << endl;
								if(player->getBoots() == nullptr) cout << "Your feet strike the ground unshielded, every step bare." << endl;
								else cout << "The boots you wear cling to your movements, alive with your stride..." << endl << *playerA << endl;
								
								cout << "------------------------------------------" << endl;
								cout << "A new pair of boots waits, eager to merge with your steps:" << endl << *placeholderA << endl;
								cout << "==========================================" << endl;
								cout << "Only one may bind to your feet and your will. Will you take them? (1 - Bind them)" << endl;
								cin >> wantsToEquip;
								
								if(wantsToEquip == 1 && playerA != nullptr){
									safe = playerA;
									
									player->setBoots(placeholderA);
									
									items[inventoryChoice - 1] = safe;
									
									cout << "------------------------------------------" << endl;
									cout << "The old boots loosen their hold and fall away." << endl;
									cout << "The new pair melds with your steps, moving as one with your body." << endl;
								}
								else if(wantsToEquip == 1 && playerA == nullptr){
									player->setBoots(placeholderA);
									items.erase(items.begin() + (inventoryChoice - 1));
									
									cout << "------------------------------------------" << endl;
									cout << "You slide your feet into the boots for the first time." << endl;
									cout << "They wrap around your stride, moving as if one with your legs and will." << endl;
								}
								
								cout << "==========================================" << endl;
								system("pause");
								break;
							default:
								break;
							}
							
						}
						else if(placeholderC != nullptr){
							int wantsToConsume = 0;
							
							cout << "==========================================" << endl;
							cout << *player;
							cout << "------------------------------------------" << endl;
							cout << "The item rests in your palm, stained by struggle." << endl;
							cout << *placeholderC << endl;
							cout << "==========================================" << endl;
							cout << "Use it, and deny death a little longer? (1 - Use it)" << endl;
							cin >> wantsToConsume;
							
							if(wantsToConsume == 1){
								player->setCurrentHp(player->getCurrentHp() + placeholderC->getHpToRecover());
								
								cout << "------------------------------------------" << endl;
								cout << "You endure the taste of iron and ash." << endl;
								cout << "Your wounds tighten, reluctantly closing." << endl;
								cout << "You regain " << placeholderC->getHpToRecover() << " vitality." << endl;
								
								if(player->getMaxHp() <= player->getCurrentHp())
									cout << "For now, death turns its gaze elsewhere." << endl;
								
								items.erase(items.begin() + (inventoryChoice - 1));
							}
							else{
								cout << "You lower it. Pain keeps you sharp." << endl;
							}
							
							cout << "==========================================" << endl;
							system("pause");
						}
					}
				}
				
			}while(inventoryChoice != 0);
		}
		
	}while(playerChoice != 1);
	
	if(cL != nullptr){
		b.battle();
	}
	
	this->playerPosition += 1;
}

// check if player finished
void Map::checkPlayerFinished(){
	if(playerPosition >= locations.size()){
		playerFinished = true;
	}
}
