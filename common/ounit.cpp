#include "common/ounit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

std::unordered_map<int, OUnit*> OUnit::units;

OUnit::OUnit(BWAPI::Unit unit)
	: GameUnit(unit)
	, dammageAttributed(0.0)
	, numberOfAttackers(0) 
{
	OUnit::units[unit->getID()] = this;
}
	
bool OUnit::willDie()
{
	int damage = (int)dammageAttributed;
	return unit->getHitPoints() < damage;
}

OUnit* OUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
		return new OUnit(unit);
}