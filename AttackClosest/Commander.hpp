#pragma once
#include <BWAPI.h>
#include <set>
#include "GameUnit.hpp"
#include "PUnit.hpp"
#include "OUnit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

using namespace BehaviorTree;

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
	BehaviorTreeNode* brain;
	
};
