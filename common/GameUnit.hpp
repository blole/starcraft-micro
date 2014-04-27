#pragma once
#include <BWAPI.h>

class GameUnit
{
public:
	// Attributes
	std::string name;
	BWAPI::Unit unit;	
	// Constructor
	GameUnit(BWAPI::Unit unit);
	// Methods
	int getHp();
	float getDps();
	BWAPI::Position getPosition();
};
