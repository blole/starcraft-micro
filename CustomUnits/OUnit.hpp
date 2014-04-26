#pragma once
#include <BWAPI.h>
#include "GameUnit.hpp"

// Remember not to use "Broodwar" in any global class constructor!

class OUnit : public GameUnit
{
public:
	// Attributes
	float dammageAttributed;
	int numberOfAttackers;
	// Constructor
	OUnit(BWAPI::Unit unit);
	// Methods
	bool willDie();
private:
	
};
