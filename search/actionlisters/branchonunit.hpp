#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search
{
	class BranchOnUnit : public ActionLister
	{
	public:
		virtual std::list<Action*> actions(const GameState* gamestate) const
		{
			const std::vector<Unit*> units = gamestate->getUnits();
			for each (Unit* unit in units)
			{
				if (unit->isAlive())
				{
					std::list<Action*> actions = unit->possibleActions(gamestate);
					if (!actions.empty())
						return actions;
				}
			}

			return std::list<Action*>();
		}
	};
}}
