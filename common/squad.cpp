#include "common/squad.hpp"
#include "common/PUnit.hpp"
#include "behaviors/moverelative.hpp"

using namespace BWAPI;

void Squad::addUnit(PUnit* unit)
{
	if (unit->squad != nullptr)
		throw std::invalid_argument("Unit already part of a squad");
	unit->squad = this;

	units.push_back(unit);
}

void Squad::removeUnit(PUnit* unit)
{
	if(unit->squad == this)
	{
		units.remove(unit);
		unit->squad = nullptr;
	}
}
