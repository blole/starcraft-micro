#pragma once
#include <BWAPI.h>
#include <set>
#include <lib/libbehavior/BehaviorTree.h>
#include "CustomUnits/GameUnit.hpp"
#include "CustomUnits/OUnit.hpp"
#include "CustomUnits/PUnit.hpp"

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
