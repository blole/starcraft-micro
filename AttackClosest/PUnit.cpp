#include "PUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
PUnit::PUnit(Unit unit)
{
	GameUnit(unit);
	target = nullptr;
}

	// Methods
bool PUnit::isIdle()
{
	return false;
}
bool PUnit::isAttacking()
{
	return false;
}

void PUnit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
}