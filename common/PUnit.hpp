#pragma once
#include <BWAPI.h>
#include "common/squad.hpp"
#include "common/GameUnit.hpp"
#include "common/OUnit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

class PUnit : public GameUnit
{
protected:
	PUnit(BWAPI::Unit unit);

public:
	// Attributes
	BehaviorTree::BehaviorTreeNode* brain;
	BWAPI::PositionOrUnit target;
	Squad* squad;
	bool hasTarget;

	// Methods
	bool isAttacking();
	void setTarget(BWAPI::PositionOrUnit newTarget);
	void attackTarget(BWAPI::PositionOrUnit newTarget, bool addToQueue);
	BWAPI::Unit getClosestEnemy();
	void attackClosestEnemyNonWorried();

	
public:
	static std::map<int, PUnit*> units;
public:
	static PUnit* get(BWAPI::Unit unit);
};
