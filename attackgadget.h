#ifndef ATTACKGADGET_H
#define ATTACKGADGET_H
#include "gadget.h"

class AttackGadget : public Gadget {
public:
	AttackGadget();
	AttackGadget(string name, float damage, int chanceOfBackfire, string type, string description);
	
	float getDamage();
	int getChanceOfBackfire();
	
	void setDamage(float damage);
	void setChanceOfBackfire(int chanceOfBackfire);
	
	friend ostream& operator<<(ostream& output, AttackGadget gadget);
private:
	float damage;
	int chanceOfBackfire;
};

#endif
