#include "common/PUnit.hpp"
#include "behaviors/AttackClosest.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

std::map<int, PUnit*> PUnit::units;

PUnit::PUnit(Unit unit)
	:GameUnit(unit)
{
	brain = (new SequentialNode())
		->addChild(new AttackClosest());
}

	// Methods
bool PUnit::isAttacking()
{
	return (this->unit->isAttacking());
}

Position PUnit::getPosition()
{
	return this->unit->getPosition();
}

Unit PUnit::getClosestEnemy()
{
	return this->unit->getClosestUnit(Filter::IsEnemy);
}

bool PUnit::exists()
{
	return this->unit->exists();
}

PUnit* PUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
	{
		PUnit* u = new PUnit(unit);
		units[id] = u;
		return u;
	}
}
