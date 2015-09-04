#include "common/unit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

int Unit::getHp()
{
	return bwapiUnit->getHitPoints();
}
float Unit::getDps()
{
	return 10.0;
}

BWAPI::Position Unit::getPosition()
{
	return bwapiUnit->getPosition();
}

bool Unit::exists()
{
	return bwapiUnit->exists();
}

bool Unit::isAttacking()
{
	//	return (unit->isAttacking());
	return (bwapiUnit->isAttackFrame() || (bwapiUnit->getGroundWeaponCooldown() != 0) || bwapiUnit->isAttacking());
}

void Unit::setTarget(PositionOrUnit newTarget)
{
	target = newTarget;
	hasTarget = true;
}

void Unit::attackTarget(PositionOrUnit newTarget, bool addToQueue = false)
{
	setTarget(newTarget);
	bwapiUnit->attack(target, addToQueue);
}
