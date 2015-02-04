#include "common/ounit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

std::map<int, OUnit*> OUnit::units;

	// Constructor
OUnit::OUnit(BWAPI::Unit unit): GameUnit(unit), dammageAttributed(0.0), numberOfAttackers(0) 
{
}
	
	// Methods
bool OUnit::willDie()
{
	int dammage = (int)this->dammageAttributed;
	return (this->unit->getHitPoints() < dammage);
}

OUnit* OUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
	{
		OUnit* u = new OUnit(unit);
		OUnit::units[id] = u;
		return u;
	}
}