#pragma once
#include <BWAPI.h>
#include <set>
#include "CustomUnits/GameUnit.hpp"
#include "CustomUnits/OUnit.hpp"
#include "CustomUnits/PUnit.hpp"

// Remember not to use "Broodwar" in any global class constructor!

class SquadManager
{
public:
	// Attributes
	BWAPI::Position positionToAttack;
	std::set<PUnit> squadUnits;
	
	// constructor
	SquadManager(std::set<PUnit> units);
	
	// Methods
	void update();
	void addUnits(std::set<PUnit> newUnits);
	void deleteUnits(std::set<PUnit> toRemoveUnits);
	void setTarget(BWAPI::Position target);
};
