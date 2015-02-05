#pragma once
#include <BWAPI.h>
#include "common/squad.hpp"
#include "common/gameunit.hpp"
#include "common/ounit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

class PUnit : public GameUnit
{
public:
	BehaviorTree::BehaviorTreeNode* brain;
	BWAPI::PositionOrUnit target;
	Squad* squad;
	bool hasTarget;
	
protected:
	PUnit(BWAPI::Unit unit);
	
public:
	bool isAttacking();
	void setTarget(BWAPI::PositionOrUnit newTarget);
	void attackTarget(BWAPI::PositionOrUnit newTarget, bool addToQueue);
	BWAPI::Unit getClosestEnemy();
	void attackClosestEnemyNonWorried();
	bool isUnderAttack();

	
public:
	static std::unordered_map<int, PUnit*> units;
	static PUnit* get(BWAPI::Unit unit);
};
