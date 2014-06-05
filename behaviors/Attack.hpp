#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

using namespace BehaviorTree;

class Attack : public BehaviorTreeNode
{
public:
	Attack();
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
private:
	bool first;
	bool hasStartAttack;
	bool hasBeenOrderToAttack;
};
