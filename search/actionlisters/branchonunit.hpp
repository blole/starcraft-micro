#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/effects/effect.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search { namespace ActionListers
{
	class BranchOnUnit : public ActionLister
	{
	public:
		virtual vector<shared_ptr<Effect>> actions(const GameState* gamestate) const override
		{
			for (auto& unit : gamestate->units)
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
			
			return { std::make_shared<AdvanceFrameEffect>(1) };
		}
	};
}}}
