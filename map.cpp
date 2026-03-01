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
				cout << "Choose an item to inspect… or disturb." << endl;
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
						Scroll* placeholderS = dynamic_cast<Scroll*>(items[inventoryChoice - 1]);;
						
						if(placeholderW != nullptr){
							Weapon* playerW = player->getWeapon();
							Weapon* safe;
							int wantsToEquip = 0;
							
							cout << "==========================================" << endl;
							cout << "Your current weapon bears the scars of your journey." << endl;
							cout << "This one has yet to taste blood in your name." << endl;
							cout << "------------------------------------------" << endl;
							cout << *playerW << " (your trusted steel)" << endl;
							cout << "------------------------------------------" << endl;
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
