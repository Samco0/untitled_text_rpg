#include "inventory.h"

//constructors
Inventory::Inventory() {
	
}
Inventory::Inventory(vector<Item*> storage) {
	this->storage = storage;
}

//getter
vector<Item*>& Inventory::getStorage() { return this->storage; }

//setter
void Inventory::setStorage(vector<Item*> storage) {this->storage = storage;}

//manipulation with items
void Inventory::addItem(Item* item) {
	if(item != nullptr) {
		storage.push_back(item);
	}
}
ostream& operator<<(ostream& os, Inventory inv) {
	Weapon* placeholderW;
	Scroll* placeholderS;
	Armor* placeholderA;
	Consumable* placeholderC;
	AttackGadget* placeholderAG;
	
	if(inv.getStorage().empty()) {
		os << "Inventory is empty.";
		return os;
	}
	
	for(int i = 0; i < inv.storage.size(); i++) {
		placeholderW = dynamic_cast<Weapon*>(inv.getStorage()[i]);
		placeholderS = dynamic_cast<Scroll*>(inv.getStorage()[i]);
		placeholderA = dynamic_cast<Armor*>(inv.getStorage()[i]);
		placeholderC = dynamic_cast<Consumable*>(inv.getStorage()[i]);
		placeholderAG = dynamic_cast<AttackGadget*>(inv.getStorage()[i]);
		
		if(inv.storage[i] != nullptr) {
			if(i!=0) cout << endl;
			os << i+1 << ". ";
			if(placeholderW != nullptr)       cout << "Weapon    | " << *placeholderW;
			else if(placeholderS != nullptr)  cout << "Scroll    | " << *placeholderS;
			else if(placeholderA != nullptr)  cout << "Armor     | " << *placeholderA;
			else if(placeholderC != nullptr)  cout << "Consum.   | " << *placeholderC;
			else if(placeholderAG != nullptr) cout << "A. Gadget | " << *placeholderAG;
		}
	}
	
	return os;
}
