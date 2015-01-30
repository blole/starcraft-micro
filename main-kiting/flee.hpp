#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include "InfluenceMap.hpp"

using namespace BehaviorTree;

class Flee : public BehaviorTreeNode
{
public:
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
	BWAPI::Position bestPos;
	bool lookForNewPos;
};
