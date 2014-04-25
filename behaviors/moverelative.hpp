#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

using namespace BehaviorTree;

class MoveRelative : public BehaviorTreeNode
{
public:
	MoveRelative(int x, int y);
	
	void init(void* agent);
	BEHAVIOR_STATUS execute(void* agent);
	
	const BehaviorTree::BehaviorTreeList* getChildren() const { return NULL; }

private:
	BWAPI::Position origin;
	BWAPI::Position offset;
};
