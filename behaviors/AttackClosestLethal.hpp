#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

using namespace BehaviorTree;

class AttackClosestLethal : public BehaviorTreeNode
{
public:
	AttackClosestLethal();
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
};
