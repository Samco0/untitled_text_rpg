#ifndef STATUSEFFECT_H
#define STATUSEFFECT_H
#include <iostream>
using namespace std;

class StatusEffect {
public:
	virtual ~StatusEffect(){}
	
	//constructor
	StatusEffect();
	StatusEffect(string name, int maxDuration, int minDuration);
	
	//getters
	string getName();
	int getMinDuration();
	int getMaxDuration();
	int getCurrentDuration();
	int getCurrentRound();
	
	//setters
	void setName(string name);
	void setMinDuration(int minDuration);
	void setMaxDuration(int maxDuration);
	void setCurrentDuration(int currentDuration);
	void setCurrentRound(int currentRound);
	
	//function to increase current round by one
	void increaseRound();
protected:
	string name;
	int minDuration, maxDuration, currentDuration, currentRound;
};

#endif
