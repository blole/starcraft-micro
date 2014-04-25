#pragma once
#include <BWAPI.h>
#include <set>

// Remember not to use "Broodwar" in any global class constructor!

class Commander
{
public:
	// Attributes
	std::set<PUnit> pAllUnits;
	std::set<OUnit> oAllUnits;
	// Constructor
	Commander();
	// Methods
	void update();
	void destroyUnit(GameUnit unit);
private:
	
};
