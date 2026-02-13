#include "armoritem.h"

//constructors
ArmorItem::ArmorItem(){
	this->name = "";
	this->description = "";
	this->damageReduction = 0;
	this->mobilityPenalty = 0;
	this->armorType = "";
}
ArmorItem::ArmorItem(string name, string description, float damageReduction, int mobilityPenalty, string armorType){
	this->name = name;
	this->description = description;
	this->damageReduction = damageReduction;
	this->mobilityPenalty = mobilityPenalty;
	this->armorType = armorType;
}

//getters
float ArmorItem::getDamageReduction(){return this->damageReduction;}
int ArmorItem::getMobilityPenalty(){return this->mobilityPenalty;}
string ArmorItem::getArmorType(){return this->armorType;}

//setters
void ArmorItem::setDamageReduction(float damageReduction){this->damageReduction = damageReduction;}
void ArmorItem::setMobilityPenalty(float mobilityPenalty){this->mobilityPenalty = mobilityPenalty;}
void ArmorItem::setArmorType(string armorType){this->armorType = armorType;}
