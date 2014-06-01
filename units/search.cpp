#include <BWAPI.h>
#include "units/search.hpp"
#include "units/action.hpp"
#include "units/possibleactions.hpp"
#include "units/unit.hpp"

using namespace Bot::Units;
using namespace BWAPI;

std::list<Action*> SearchUCT::search(GameState* gamestate, PossibleActions* possibleActions) const
{
	std::vector<Action*> bestActions;
	//abort();
	std::list<Action*> a = possibleActions->actions(gamestate);
	Broodwar->drawTextScreen(200, 100,  "possible actions: %d", a.size());
	return a;
	//return bestActions;
}
