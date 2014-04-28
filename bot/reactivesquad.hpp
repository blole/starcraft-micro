#pragma once
#include "common/squad.hpp"
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include <list>

using namespace BehaviorTree;

class ReactiveSquad : public Squad
{
public:
	std::function<BehaviorTreeNode*()> unitBrain;

public:
	ReactiveSquad(std::function<BehaviorTreeNode*()> unitBrain);
	
	void onFrame();
	
	virtual void addUnit(PUnit* unit);
	virtual void removeUnit(PUnit* unit);
};
