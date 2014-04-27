#pragma once
#include <BWAPI.h>
#include "common/GameUnit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

class PUnit : public GameUnit
{
protected:
	PUnit(BWAPI::Unit unit);

public:
	// Attributes
	BehaviorTree::BehaviorTreeNode* brain;
	BWAPI::PositionOrUnit target;

	// Methods
	bool isAttacking();
	void setTarget(BWAPI::PositionOrUnit newTarget);
	void attackTarget(BWAPI::PositionOrUnit newTarget);
	BWAPI::Unit getClosestEnemy();

	
protected:
	static std::map<int, PUnit*> units;
public:
	static PUnit* get(BWAPI::Unit unit);
};
