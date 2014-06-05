#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include "common/InfluenceMap.hpp"

using namespace BehaviorTree;

class Flee : public BehaviorTreeNode
{
public:
	Flee();
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
	BWAPI::Position bestPos;
	bool lookForNewPos;
};
