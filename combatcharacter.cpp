#include "combatcharacter.h"

//constructors
CombatCharacter::CombatCharacter(){
	maxHp = 100.0f;
	currentHp = 100.0f;
	dmg = 10.0f;
	level = 1;
	speed = 10;
	critChance = 5;
	critValue = 1.5f;
	
	for(int i=0;i<4;i++){
		spells[i] = nullptr;
		statusEffect[i] = nullptr;
	}
}
CombatCharacter::CombatCharacter(string name, float maxHp, float currentHp, float dmg, int level, int speed, int critChance, float critValue, Spell* spells[4]){
	this->name = name;
	this->maxHp = maxHp;
	this->currentHp = currentHp;
	this->dmg = dmg;
	this->level = level;
	this->speed = speed;
	this->critChance = critChance;
	this->critValue = critValue;
	
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
		statusEffect[i] = nullptr;
	}
}

//getters
float CombatCharacter::getMaxHp(){return maxHp;}
float CombatCharacter::getCurrentHp(){return currentHp;}
float CombatCharacter::getDmg(){return dmg;}
int CombatCharacter::getLevel(){return level;}
int CombatCharacter::getSpeed(){return speed;}
int CombatCharacter::getCritChance(){return critChance;}
float CombatCharacter::getCritValue(){return critValue;}
Spell** CombatCharacter::getSpells(){return spells;}
StatusEffect** CombatCharacter::getStatusEffect(){return statusEffect;}

//setters
void CombatCharacter::setMaxHp(float maxHp){this->maxHp = maxHp;}
void CombatCharacter::setCurrentHp(float currentHp){this->currentHp = currentHp;}
void CombatCharacter::setDmg(float dmg){this->dmg = dmg;}
void CombatCharacter::setLevel(int level){this->level = level;}
void CombatCharacter::setSpeed(int speed){this->speed = speed;}
void CombatCharacter::setCritChance(int critChance){this->critChance = critChance;}
void CombatCharacter::setCritValue(float critValue){this->critValue = critValue;}
void CombatCharacter::setSpells(Spell* spells[4]){
	for(int i=0;i<4;i++){
		this->spells[i] = spells[i];
	}
}
void CombatCharacter::setSpell(int index, Spell* spell){
	this->spells[index] = spell;
}
void CombatCharacter::setStatusEffect(StatusEffect* statusEffect[4]){
	for(int i=0;i<4;i++){
		this->statusEffect[i] = statusEffect[i];
	}
}

//statuseffect connected functions
void CombatCharacter::addStatusEffect(StatusEffect* statusEffect){
	for(int i=0;i<4;i++){
		//goes through the function
		//if the function doesnt find an empty space, throws out a different text and player doesnt gain the effect
		if(this->statusEffect[i] == nullptr){
			this->statusEffect[i] = statusEffect;
			break;
		}
	}
	
}

void CombatCharacter::removeStatusEffect(int arrayIndex){
	//this function turns a statuseffect into a null pointer, and sorts it to the end of the array
	this->statusEffect[arrayIndex] = nullptr;
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3 - i; j++){
			if(this->statusEffect[j] == nullptr && this->statusEffect[j + 1] != nullptr){
				this->statusEffect[j] = this->statusEffect[j + 1];
				this->statusEffect[j + 1] = nullptr;
			}
		}
	}
}

void CombatCharacter::checkStatusEffects(){
	// this function checks if any of the statuses in the array can be turned into a nullptr, if yes, then uses removeStatusEffect
	for(int i = 0; i < 4; i++){
		if(this->statusEffect[i] != nullptr &&
		   this->statusEffect[i]->getCurrentRound() >= this->statusEffect[i]->getCurrentDuration()){
			removeStatusEffect(i);
			i--;
		}
	}
}

//output
ostream& operator<<(ostream& output, CombatCharacter character){
	cout << character.getName() << endl;
	cout << " -> Vitality: " << character.getCurrentHp() << "/" << character.getMaxHp() << "hp" << endl;
	cout << " -> Wisdom: " << character.getLevel() << ". level"<< endl;
	cout << " -> Agility: " << character.getSpeed() << endl;
	return output;
}

