#include "search/unit.hpp"
#include "search/gamestate.hpp"
#include "search/units/marine.hpp"

using namespace Bot::Search;

Unit* Unit::create(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
{
	Unit* unit;
	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		unit = new Terran_Marine(bwapiUnit, id);
	default:
		throw std::runtime_error("only marines supported for MCTS so far.");
	}

	unit->firstFrameInitToAddAlreadyActiveEffects(state);

	return unit;
}
