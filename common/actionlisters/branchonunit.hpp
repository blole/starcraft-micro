#pragma once
#include "common/actionlisters/actionlister.hpp"
#include "common/effects/effect.hpp"
#include "common/units/unit.hpp"

namespace Bot { namespace Search { namespace ActionListers
{
	struct BranchOnUnit : ActionLister
	{
		virtual vector<shared_ptr<Effect>> operator()(const GameState& gamestate) const override
		{
			for (auto& unit : gamestate.units)
			{
				if (unit->isAlive())
				{
					vector<shared_ptr<Effect>> actions = unit->possibleActions(gamestate);

					if (actions.size() > 2)
						actions.erase(actions.begin()+2, actions.end());

					if (!actions.empty())
						return actions;
				}
			}
			
			return { make_shared<AdvanceFrameEffect>(1) };
		}
	};
}}}
