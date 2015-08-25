#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/effects/effect.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search { namespace ActionListers
{
	class BranchOnUnit : public ActionLister
	{
	public:
		virtual std::vector<Effect*> actions(const GameState* gamestate) const override
		{
			for (auto& unit : gamestate->units)
			{
				if (unit->isAlive())
				{
					std::vector<Effect*> actions = unit->possibleActions(gamestate);

					if (actions.size() > 2)
					{
						std::vector<Effect*>::iterator element2 = actions.begin();
						std::advance(element2, 2);
						actions.erase(element2, actions.end());
					}

					if (!actions.empty())
						return actions;
				}
			}
			
			return { new AdvanceFrameEffect(1) };
		}
	};
}}}
