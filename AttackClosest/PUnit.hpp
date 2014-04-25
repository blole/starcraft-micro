#pragma once
#include <BWAPI.h>
#include "GameUnit"

// Remember not to use "Broodwar" in any global class constructor!

class PUnit : public GameUnit
{
public:
	// Attributes
	BWAPI::PositionOrUnit target;
	// Constructor
	PUnit();
	// Methods
	void isIdle();
	void isAttacking();
private:
};
