#pragma once
#include <BWAPI.h>
#include "GameUnit"

// Remember not to use "Broodwar" in any global class constructor!

class OUnit : public GameUnit
{
public:
	// Attributes
	float dammageAttributed;
	int numberOfAttackers;
	// Constructor
	OUnit();
	// Methods
	bool willDie();
private:
	
};
