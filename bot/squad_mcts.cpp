#include "BWAPI.h"
#include "bot/squad_mcts.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/searchers/searcheruct.hpp"
#include "search/actionlisters/branchonunit.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Search;

void SquadMCTS::onFrame()
{
	static Searcher* searchAlgorithm = new SearcherUCT();
	static ActionLister* actionlister = new BranchOnUnit();

	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::vector<BWAPI::Unit> playerUnits;
	std::vector<BWAPI::Unit> enemyUnits;

	for each(PUnit* unit in units)
	{
		playerUnits.push_back(unit->unit);
		for each (BWAPI::Unit enemyUnit in unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
		{
			if (std::find(enemyUnits.begin(), enemyUnits.end(), enemyUnit) == enemyUnits.end())
				enemyUnits.push_back(enemyUnit);
		}
	}

	GameState state(playerUnits, enemyUnits);

	if (!state.isTerminal())
	{
		try {
			int gameframe = BWAPI::Broodwar->getFrameCount();
			std::list<Action*> actions = searchAlgorithm->search(&state, actionlister);

			for each (Action* action in actions)
			{
				if (action->isPlayerAction(&state))
					action->executeOrder(&state);
			}
		} catch (const std::runtime_error&) {
			throw;
		} catch (const std::exception&) {
			throw;
		} catch (...) {
			abort();
		}
	}
}
