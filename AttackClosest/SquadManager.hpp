#pragma once
#include <BWAPI.h>
#include <set>

// Remember not to use "Broodwar" in any global class constructor!

class SquadManager
{
public:
	// Attributes
	BWAPI::Position positionToAttack;
	std:set<PUnit> squadUnits;
	
	// constructor
	SquadManager(std::set<PUnit> units);
	
	// Methods
	void update();
	void addUnits(std::set<PUnit> newUnits);
	void deleteUnits(std::set<PUnit> toRemoveUnits);
};
