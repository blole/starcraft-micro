#pragma once
#include <BWAPI.h>
#include "common/gameunit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

namespace Bot
{
	class Squad;

	class PUnit : public GameUnit
	{
	public:
		BehaviorTree::BehaviorTreeNode* brain;
		BWAPI::PositionOrUnit target;
		Squad* squad;
		bool hasTarget;
	
	protected:
		PUnit(BWAPI::Unit unit);
	
	public:
		bool isAttacking();
		void setTarget(BWAPI::PositionOrUnit newTarget);
		void attackTarget(BWAPI::PositionOrUnit newTarget, bool addToQueue);

	
	public:
		static std::unordered_map<int, PUnit*> units;
		static PUnit* get(BWAPI::Unit unit);
	};
}
