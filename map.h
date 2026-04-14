#ifndef MAP_H
#define MAP_H

#include "multiplecombatlocation.h"
#include "groupcombatlocation.h"
#include "treasurelocation.h"
#include "player.h"
#include "battlemanager.h"
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// Class representing the game map
class Map {
public:
	// Constructors
	Map();
	Map(string name, Player* player, vector<Location*> locations);
	Map(string name, Player* player);
	
	// Getters
	string getName();
	Player* getPlayer();
	vector<Location*>& getLocations();
	int getPlayerPosition();
	bool getPlayerFinished();
	
	// Setters
	void setName(string name);
	void setPlayer(Player* player);
	void setLocations(vector<Location*> locations);
	void setPlayerPosition(int playerPosition);
	void setPlayerFinished(bool playerFinished);
	
	// Working with locations
	void addLocation(Location* location);
	void removeLocation(int index);
	
	// Output
	friend ostream& operator<<(ostream& output, Map m);
	
	// Player movement
	void movePlayer();
	
	// Check if player finished the map
	void checkPlayerFinished();
	
	// Generate map based on difficulty pools
	void generateMap(vector<Location*>& easyLocations, vector<Location*>& mediumLocations, vector<Location*>& hardLocations, vector<Location*>& bossLocations, int easyCount, int mediumCount, int hardCount);
	
private:
	string name;                  // map name
	Player* player;               // the player on this map
	vector<Location*> locations;  // all locations on the map
	int playerPosition;           // index of player's current location
	bool playerFinished;          // true if player reached end
};

#endif
