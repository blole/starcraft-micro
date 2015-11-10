#pragma once
#include "common/actionlisters/actionlister.hpp"
#include "common/effects/effect.hpp"
#include "common/units/unit.hpp"

namespace Bot { namespace ActionListers
{
	struct BranchOnUnit : ActionLister
	{
		virtual vector<shared_ptr<Effect>> operator()(const GameState& state) const override
		{
			for (auto& unit : state.units)
			{
				if (unit->isAlive())
				{
					vector<shared_ptr<Effect>> actions = unit->possibleActions(state);

					if (!actions.empty())
						return actions;
				}
			}
			
			return {};
		}
	};
}}
