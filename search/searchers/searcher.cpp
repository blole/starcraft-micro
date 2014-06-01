#include <BWAPI.h>
#include "search/searchers/searcher.hpp"
#include "search/actions/action.hpp"
#include "search/actionlisters/actionlister.hpp"
#include "search/units/unit.hpp"

using namespace Bot::Search;
using namespace BWAPI;

std::list<Action*> SearchUCT::search(GameState* gamestate, ActionLister* possibleActions) const
{
	std::vector<Action*> bestActions;
	//abort();
	std::list<Action*> a = possibleActions->actions(gamestate);
	Broodwar->drawTextScreen(200, 100,  "possible actions: %d", a.size());
	return a;
	//return bestActions;
}
