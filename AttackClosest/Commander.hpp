#pragma once
#include <BWAPI.h>
#include <set>
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"
#include "common/OUnit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

using namespace BehaviorTree;

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
};
