#ifndef DEFENSEITEM_H
#define DEFENSEITEM_H
#include "item.h"

class ArmorItem : public Item{
public:
	virtual ~ArmorItem() {}
	
	//constructors
	ArmorItem();
	ArmorItem(string name, string description, float damageReduction, int mobilityPenalty, string armorType);
	
	//getters
	float getDamageReduction();
	int getMobilityPenalty();
	string getArmorType();
	
	//setters
	void setDamageReduction(float damageReduction);
	void setMobilityPenalty(float mobilityPenalty);
	void setArmorType(string armorType);
protected:
	float damageReduction;
	int mobilityPenalty;
	//in case the armor is "Heavy"
	//also if the mobility penalty caps at speed 1, thats the slowest you can get
	//(because how would it be possible to do anything at 0 speed (youd just stand paused in time))
	string armorType;
	//"helmet", "chestplate", "gloves", "leggings", "boots"
};

#endif
