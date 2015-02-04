#pragma once
#include <BWAPI.h>
#include <list>
#include "common/generalallunitssinglesquad.hpp"
#include "influencemap.hpp"

class GeneralKiting : public GeneralAllUnitsSingleSquad
{
public:
	explicit GeneralKiting(std::function<Squad*()>& newSquad)
		: GeneralAllUnitsSingleSquad(newSquad)
	{}
	explicit GeneralKiting(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
		: GeneralAllUnitsSingleSquad(unitBrain)
	{}
	
	
public:
	virtual void GeneralKiting::onInit()
	{
		InfluenceMap::init();
		GeneralAllUnitsSingleSquad::onStart();
	}

	virtual void GeneralKiting::onFrame()
	{
		InfluenceMap::update();
		GeneralAllUnitsSingleSquad::onFrame();
	}
};
