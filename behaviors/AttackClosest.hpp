#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

using namespace BehaviorTree;

class AttackClosest : public BehaviorTreeNode
{
public:
	AttackClosest();
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
};
