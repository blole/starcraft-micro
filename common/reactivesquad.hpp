#pragma once
#include "common/squad.hpp"
#include "common/punit.hpp"
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include <list>

namespace Bot
{
	class ReactiveSquad : public Squad
	{
	public:
		std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain;
	
	public:
		ReactiveSquad(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
			: unitBrain(unitBrain)
		{}
	
	
	public:
		void onFrame() override
		{
			units.remove_if([](PUnit* unit){ return !unit->exists(); });

			for (PUnit* unit : units)
				unit->brain->execute(unit);
		}
	
		virtual void addUnit(PUnit* unit) override
		{
			Squad::addUnit(unit);
			delete unit->brain;
			unit->brain = unitBrain();
		}
		virtual void removeUnit(PUnit* unit) override
		{
			Squad::removeUnit(unit);
			delete unit->brain;
			unit->brain = nullptr;
		}
	};
}
