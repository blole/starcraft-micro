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
			Squad::onFrame();

			for (Unit* unit : units())
				unit->brain->execute(unit);
		}

		virtual void add(Unit& unit) override
		{
			Squad::add(unit);
			delete unit.brain;
			unit.brain = unitBrain();
		}
		virtual void remove(Unit& unit) override
		{
			Squad::remove(unit);
			delete unit.brain;
			unit.brain = nullptr;
		}
	};
}}
