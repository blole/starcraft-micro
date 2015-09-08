#include "common/units/unit.hpp"
#include "common/units/marine.hpp"

using namespace Bot::Search;
using namespace std;

Unit* Unit::create(BWAPI::Unit bwapiUnit)
{
	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		return new Terran_Marine(bwapiUnit);
	default:
		throw runtime_error("only marines supported for MCTS so far.");
	}
}
