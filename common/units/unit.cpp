#include "common/units/unit.hpp"
#include "common/units/marine.hpp"

using namespace Bot::Search;
using namespace std;

unique_ptr<Unit> Unit::create(const GameState& state, BWAPI::Unit bwapiUnit, id_t id)
{
	unique_ptr<Unit> unit;
	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		unit = make_unique<Terran_Marine>(bwapiUnit, id);
		break;
	default:
		throw runtime_error("only marines supported for MCTS so far.");
	}

	return unit;
}