#ifndef MAP_H
#define MAP_H
#include "combatlocation.h"
#include "player.h"
#include "battlemanager.h"
#include <vector>
using namespace std;

class Map {
public:
	// constructors
	Map();
	Map(string name, Player* player, vector<Location*> locations);
	
	// getters
	string getName();
	Player* getPlayer();
	vector<Location*>& getLocations();
	int getPlayerPosition();
	bool getPlayerFinished();
	
	// setters
	void setName(string name);
	void setPlayer(Player* player);
	void setLocations(vector<Location*> locations);
	void setPlayerPosition(int playerPosition);
	void setPlayerFinished(bool playerFinished);
	
	// working with the array
	void addLocation(Location* location);
	void removeLocation(int index);
	
	// typing out map
	friend ostream& operator<<(ostream& output, Map m);
	
	// function related to moving the player on the map
	void movePlayer();
	
	// function that checks if player finished the map
	void checkPlayerFinished();
	
private:
	string name;
	Player* player;
	vector<Location*> locations;
	int playerPosition;
	bool playerFinished;
};

#endif
