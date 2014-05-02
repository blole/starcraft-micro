#include "common/PUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

std::map<int, PUnit*> PUnit::units;

PUnit::PUnit(Unit unit)
	: GameUnit(unit)
	, target(nullptr)
	, brain(nullptr)
	, squad(nullptr)
{
}

	// Methods
bool PUnit::isAttacking()
{
	return (this->unit->isAttackFrame() || (this->unit->getGroundWeaponCooldown() != 0) || this->unit->isAttacking());
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
}

void PUnit::attackTarget(PositionOrUnit newTarget, bool addToQueue = false)
{
	target = newTarget;
	this->unit->attack(target,addToQueue);
}

Unit PUnit::getClosestEnemy()
{
	return this->unit->getClosestUnit(Filter::IsEnemy);
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
