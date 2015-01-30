#pragma once
#include <BWAPI.h>
#include <list>
#include "common/general.hpp"

class GeneralAllUnitsSingleSquad : public General
{
public:
	explicit GeneralAllUnitsSingleSquad(std::function<Squad*()>& newSquad)
		: General(newSquad)
	{}
	explicit GeneralAllUnitsSingleSquad(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
		: General(unitBrain)
	{}
	
	
public:
	virtual void onFrame();
};
