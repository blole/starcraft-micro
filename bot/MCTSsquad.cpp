#include "BWAPI.h"
#include "bot/MCTSsquad.hpp"
#include "behaviors/moverelative.hpp"
#include "units/gamestate.hpp"
#include "units/action.hpp"
#include "common/PUnit.hpp"
#include <exception>
#include <vector>
#include <set>

using namespace BWAPI;
using namespace Bot::Units;

void MCTSsquad::onFrame()
{
	units.remove_if([](PUnit* unit){ return !unit->exists(); });
	
	std::set<BWAPI::Unit> bwapiUnits;

	for each(PUnit* unit in units)
	{
		bwapiUnits.insert(unit->unit);
		for each (BWAPI::Unit enemy in unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
			bwapiUnits.insert(enemy);
	}

	GameState state(std::vector<BWAPI::Unit>(bwapiUnits.begin(), bwapiUnits.end()));

	static std::vector<Action*> actions = state.search();

	for each (Action* action in actions)
	{
		action->executeOrder(&state);
	}
}
