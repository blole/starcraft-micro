#include "PUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
PUnit::PUnit(Unit unit) : GameUnit(unit)
{
	target = nullptr;
}

	// Methods
bool PUnit::isAttacking()
{
	return (this->unit->isAttacking());
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
}

void PUnit::attackTarget(PositionOrUnit newTarget)
{
		target = newTarget;
		this->unit->attack(target);
}

Unit PUnit::getClosestEnnemy()
{
	return this->unit->getClosestUnit(Filter::IsEnemy);
}
