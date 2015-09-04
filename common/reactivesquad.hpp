#pragma once
#include "common/squad.hpp"
#include "common/unit.hpp"
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
			units.remove_if([](Unit* unit){ return !unit->exists(); });

			for (Unit* unit : units)
				unit->brain->execute(unit);
		}
	
		virtual void addUnit(Unit& unit) override
		{
			Squad::addUnit(unit);
			delete unit.brain;
			unit.brain = unitBrain();
		}
		virtual void removeUnit(Unit& unit) override
		{
			Squad::removeUnit(unit);
			delete unit.brain;
			unit.brain = nullptr;
		}
	};
}
