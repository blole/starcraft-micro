#pragma once
#include <BWAPI.h>

// Remember not to use "Broodwar" in any global class constructor!

class GameUnit
{
public:
	// Attributes
	int hp;
	float dps;
	string name;
	BWAPI::Position position;
	// Constructor
	GameUnit();
	GameUnit(BWAPI::Unit unit);
	// Methods
	void update();
private:
	// Attributes
	BWAPI::Unit unit;	
};
