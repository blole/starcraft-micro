#pragma once
#include <BWAPI.h>
#include <set>
#include "common/unit.hpp"
#include "common/squad.hpp"

namespace Bot
{
	class SquadManager : public Squad
	{
	public:
		BWAPI::PositionOrUnit positionToAttack;
		BWAPI::Position positionSquad;
		std::set<Unit*> squadUnits;
		BehaviorTree::BehaviorTreeNode* brain;
	
	public:
		SquadManager();
	
	public:
		virtual void onFrame() override;
		void setTarget(BWAPI::PositionOrUnit target);
		bool isInPosition();
	};
}
