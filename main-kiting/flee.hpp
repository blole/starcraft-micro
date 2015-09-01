#pragma once
#include "common/common.hpp"
#include <lib/libbehavior/BehaviorTree.h>

class Flee : public BehaviorTree::BehaviorTreeNode
{
public:
	void init(void* agent);
	BehaviorTree::BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
	BWAPI::Position bestPos;
	bool lookForNewPos;
};
