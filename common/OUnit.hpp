#pragma once
#include <BWAPI.h>
#include "common/GameUnit.hpp"

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
