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
	
	const BehaviorTree::BehaviorTreeList* getChildren() const { return NULL; }

private:
	BWAPI::Unit target;
};
