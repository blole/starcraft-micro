#pragma once
#include <BWAPI.h>
#include <set>
#include "common/GameUnit.hpp"
#include "common/OUnit.hpp"
#include "common/PUnit.hpp"

class SquadManager
{
public:
	// Attributes
	BWAPI::Position positionToAttack;
	BWAPI::Position positionSquad;
	std::set<PUnit> squadUnits;
	BehaviorTree::BehaviorTreeNode* squadBrain;
	
	// constructor
	SquadManager(std::set<PUnit*> units);
	
	// Methods
	void update();
	void addUnits(std::set<PUnit*> newUnits);
	bool destroyUnit(BWAPI::Unit unit);
	void setTarget(BWAPI::PositionOrUnit target);
	bool isInPosition();
};
