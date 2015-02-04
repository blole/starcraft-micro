#pragma once
#include <BWAPI.h>
#include <set>
#include "common/gameunit.hpp"
#include "common/ounit.hpp"
#include "common/punit.hpp"
#include "common/squad.hpp"

class SquadManager : public Squad
{
public:
	// Attributes
	BWAPI::PositionOrUnit positionToAttack;
	BWAPI::Position positionSquad;
	std::set<PUnit*> squadUnits;
	BehaviorTree::BehaviorTreeNode* brain;
	
	// constructor
	SquadManager();
	
	// Methods
	void onFrame();
	void setTarget(BWAPI::PositionOrUnit target);
	bool isInPosition();
};
