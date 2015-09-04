#include "common/punit.hpp"
#include <iostream>
#include "common/squad.hpp"

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

std::unordered_map<int, PUnit*> PUnit::units;

PUnit::PUnit(Unit unit)
	: GameUnit(unit)
	, target(nullptr)
	, brain(nullptr)
	, squad(nullptr)
	, hasTarget(false)
{
	PUnit::units[unit->getID()] = this;
}

bool PUnit::isAttacking()
{
	//	return (unit->isAttacking());
	return (unit->isAttackFrame() || (unit->getGroundWeaponCooldown() != 0) || unit->isAttacking());
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
	hasTarget = true;
}

void PUnit::attackTarget(PositionOrUnit newTarget, bool addToQueue = false)
{
	setTarget(newTarget);
	unit->attack(target,addToQueue);
}

PUnit* PUnit::get(BWAPI::Unit unit)
{
	int id = unit->getID();

	auto iter = units.find(id);
	if (iter != units.end())
		return iter->second;
	else
		return new PUnit(unit);
}
