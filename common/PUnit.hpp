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

	// Methods
	bool isAttacking();
	BWAPI::Position getPosition();
	BWAPI::Unit getClosestEnemy();
	bool exists();
	
	
protected:
	static std::map<int, PUnit*> units;
public:
	static PUnit* get(BWAPI::Unit unit);
};
