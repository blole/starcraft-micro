#include "BWAPI.h"
#include "bot/MCTSsquad.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/searchers/searcher.hpp"
#include "search/actionlisters/actionlister.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Search;

void MCTSsquad::onFrame()
{
	static Searcher* searchAlgorithm = new SearchUCT();
	static ActionLister* possibleActions = new BranchOnUnit();

	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::set<BWAPI::Unit> bwapiUnits;

	for each(PUnit* unit in units)
	{
		bwapiUnits.insert(unit->unit);
		for each (BWAPI::Unit enemy in unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
			bwapiUnits.insert(enemy);
	}

	GameState state(std::vector<BWAPI::Unit>(bwapiUnits.begin(), bwapiUnits.end()));

	try {
		std::list<Action*> actions = searchAlgorithm->search(&state, possibleActions);

		for each (Action* action in actions)
		{
			action->executeOrder(&state);
		}
	} catch(const std::runtime_error& e) {
		abort();
	} catch(const std::exception& e) {
		abort();
	} catch(...) {
		abort();
	}
}
