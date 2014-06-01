#include "search/actionlisters/actionlister.hpp"
#include "search/actions/action.hpp"
#include "search/units/unit.hpp"

using namespace Bot::Search;

std::list<Action*> BranchOnUnit::actions(const GameState* gamestate) const
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
