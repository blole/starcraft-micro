#include "common/OUnit.hpp"
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
	return false;
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
		units[id] = u;
		return u;
	}
}