#pragma once
#include <BWAPI.h>
#include <lib/libbehavior/BehaviorTree.h>

using namespace BehaviorTree;

class Commander
{
public:
	Commander();
	void onFrame();

private:
	BehaviorTreeNode* brain;
};
