#include "BWAPI.h"
#include "squad_mcts.hpp"
#include "behaviors/moverelative.hpp"
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/effect.hpp"
#include "search/searchers/searcheruct.hpp"
#include "search/actionlisters/branchonunit.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Search;

void SquadMCTS::onFrame()
{
	static Searcher* searchAlgorithm = new UCT::SearcherUCT();
	static ActionLister* actionlister = new BranchOnUnit();

	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::vector<BWAPI::Unit> playerUnits;
	std::vector<BWAPI::Unit> enemyUnits;

	for (PUnit* unit : units)
	{
		playerUnits.push_back(unit->unit);
		for (auto& u : unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
		{
			if (std::find(enemyUnits.begin(), enemyUnits.end(), u) == enemyUnits.end())
				enemyUnits.push_back(u);
		}
	}

	GameState state(playerUnits, enemyUnits);

	if (!state.isTerminal())
	{
		try {
			int gameframe = BWAPI::Broodwar->getFrameCount();
			std::vector<Effect*> actions = searchAlgorithm->search(&state, actionlister);

			for (Effect* action : actions)
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
