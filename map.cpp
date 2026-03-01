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
		cout << "Adventurer you made it! We escaped from " << this->name << "!" << endl;
		cout << "==========================================" << endl;
		return;
	}
	
	do{
		playerChoice = -1;
		system("cls");
		cout << *this << endl;
		cout << "==========================================" << endl;
		cout << "Adventurer! Here are the choice you can make:" << endl;
		cout << "1. Advance to the next location" << endl;
		cout << "2. Open inventory" << endl;
		cout << "==========================================" << endl;
		cout << "What is your choice? " << endl;
		cin >> playerChoice;
		
		if(playerChoice == 2){
			int inventoryChoice = -1;
			do{
				system("cls");

				cout << "==========================================" << endl;
				cout << "Your inventory:" << endl;
				cout << "------------------------------------------" << endl;
				
				cout << this->player->getInventory() << endl;
				
				cout << "==========================================" << endl;
				cout << "Press 0 to exit inventory" << endl;
				cout << "Press item number to show details" << endl;
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
						cout << "==========================================" << endl;
						cout << "Item details:" << endl;
						cout << "------------------------------------------" << endl;
						
						cout << "==========================================" << endl;
						system("pause");
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
