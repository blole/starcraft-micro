#pragma once
#include "common/squads/squad.hpp"
#include "common/unit.hpp"
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>
#include <list>

namespace Bot { namespace Squads
{
	struct Reactive : Squad
	{
		function<BehaviorTree::BehaviorTreeNode*()> unitBrain;


		Reactive(function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
			: unitBrain(unitBrain)
		{}


		void onFrame() override
		{
			units.remove_if([](Unit* unit){ return !unit->bwapiUnit->exists(); });

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
}}
