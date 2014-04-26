#pragma once
#include <BWAPI.h>

// Remember not to use "Broodwar" in any global class constructor!

class GameUnit
{
public:
	// Attributes
	std::string name;
	// Constructor
	GameUnit(BWAPI::Unit unit);
	// Methods
	int getHp();
	float getDps();
	BWAPI::Position getPosition();
	bool exists();
protected:
	// Attributes
	BWAPI::Unit unit;	
};
