#pragma once
#include "common/squad.hpp"
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include <list>

class ReactiveSquad : public Squad
{
public:
	std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain;
	
public:
	ReactiveSquad(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain);
	
	
public:
	void onFrame();
	
	virtual void addUnit(PUnit* unit);
	virtual void removeUnit(PUnit* unit);
};
