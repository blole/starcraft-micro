#pragma once
#include <BWAPI.h>
#include "common/gameunit.hpp"

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

public:
	static std::map<int, OUnit*> units;
public:
	static OUnit* get(BWAPI::Unit unit);
	
};
