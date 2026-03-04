#ifndef STATUSEFFECT_H
#define STATUSEFFECT_H

#include <iostream>
using namespace std;

// Base class for any status effect (like poison, burn, buff)
class StatusEffect {
public:
	virtual ~StatusEffect() {}
	
	// Constructors
	StatusEffect();
	StatusEffect(string name, int maxDuration, int minDuration);
	
	// Getters
	string getName();
	int getMinDuration();
	int getMaxDuration();
	int getCurrentDuration();
	int getCurrentRound();
	
	// Setters
	void setName(string name);
	void setMinDuration(int minDuration);
	void setMaxDuration(int maxDuration);
	void setCurrentDuration(int currentDuration);
	void setCurrentRound(int currentRound);
	
	// Increase current round by 1
	void increaseRound();
	
protected:
	string name;            // status effect name
	int minDuration;        // minimum duration in rounds
	int maxDuration;        // maximum duration in rounds
	int currentDuration;    // how many rounds it lasts currently
	int currentRound;       // current round counter
};

#endif
