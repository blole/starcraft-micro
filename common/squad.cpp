#include "common/squad.hpp"
#include "common/punit.hpp"
#include "behaviors/moverelative.hpp"

using namespace BWAPI;
using namespace Bot;

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

void Squad::displaySquadLinks()
{
	for(auto i=units.begin(); i!=units.end();i++)
	{
		for(auto j=units.begin(); j!=units.end();j++)
		{
			Broodwar->drawLineMap((*i)->getPosition(),(*j)->getPosition(),Color(0,255,0));
		}
	}
}
