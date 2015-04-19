#include "BWAPI.h"
#include "squad_abcd.hpp"
#include "behaviors/moverelative.hpp"
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/searchers/searcherabcd.hpp"
#include "search/actionlisters/branchonplayer.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Search;

void SquadABCD::onFrame()
{
	static Searcher* searchAlgorithm = new SearcherABCD();
	static BranchOnPlayer* possibleActions = new BranchOnPlayer();

	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::vector<BWAPI::Unit> playerUnits;
	std::vector<BWAPI::Unit> enemyUnits;

	for (PUnit* unit : units)
	{
		playerUnits.push_back(unit->unit);
		for (BWAPI::Unit enemyUnit : unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
		{
			if (std::find(enemyUnits.begin(), enemyUnits.end(), enemyUnit) == enemyUnits.end())
				enemyUnits.push_back(enemyUnit);
		}
	}

	GameState state(playerUnits, enemyUnits);


	if (!state.isTerminal())
	{
		try {
			std::list<Action*> actions = searchAlgorithm->search(&state, possibleActions);

			for (Action* action : actions)
			{
				if (action->isPlayerAction(&state))
					action->executeOrder(&state);
			}
		} catch(const std::runtime_error&) {
			throw;
		} catch(const std::exception&) {
			throw;
		} catch(...) {
			abort();
		}
	}
}
