#pragma once
#include <BWAPI.h>
#include <set>
#include "common/GameUnit.hpp"
#include "common/OUnit.hpp"
#include "common/PUnit.hpp"
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
