#pragma once
#include <BWAPI.h>
#include "GameUnit.hpp"

// Remember not to use "Broodwar" in any global class constructor!

class PUnit : public GameUnit
{
public:
	// Attributes
	BWAPI::PositionOrUnit target;
	// Constructor
	PUnit(BWAPI::Unit unit);
	// Methods
	bool isIdle();
	bool isAttacking();
	void setTarget(BWAPI::PositionOrUnit newTarget);
private:
};
