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
void Inventory::removeItem(int index) {
	if(index < 0 || index >= storage.size()) {
		return;
	}
	
	delete storage[index];
	storage.erase(storage.begin() + index);
}
ostream& operator<<(ostream& os, Inventory inv) {
	Weapon* placeholderW;
	
	if(inv.getStorage().empty()) {
		os << "Inventory is empty.";
		return os;
	}
	
	for(int i = 0; i < inv.storage.size(); i++) {
		placeholderW = dynamic_cast<Weapon*>(inv.getStorage()[i]);
		
		if(inv.storage[i] != nullptr) {
			os << i << ". " << inv.getStorage()[i]->getName();
			if(placeholderW != nullptr) cout << " | A " << placeholderW->getName() << ", " << placeholderW->getDmg() << ", " << placeholderW->getDescription();
		}
	}
	
	return os;
}
