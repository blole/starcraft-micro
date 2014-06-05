#pragma once
#include <BWAPI.h>
#include <set>
#include "common/GameUnit.hpp"
#include "common/OUnit.hpp"
#include "common/PUnit.hpp"

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
