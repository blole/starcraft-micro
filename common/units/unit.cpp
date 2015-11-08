#include "common/units/unit.hpp"
#include "common/units/marine.hpp"

using namespace Bot;

void Unit::onFrame()
{
	hp_ = bwapiUnit->getHitPoints();
	pos = bwapiUnit->getPosition();
	isMoving = bwapiUnit->isMoving();
	groundWeaponCooldown = bwapiUnit->getGroundWeaponCooldown();

	if (bwapiUnit->isStartingAttack())
		moveCooldown = 6;
	else if (moveCooldown > 0)
		moveCooldown--;
}

void Unit::simulateOneFrameForward(GameState& state)
{
	if (groundWeaponCooldown > 0)
		groundWeaponCooldown--;
	if (moveCooldown > 0)
		moveCooldown--;
}


Unit* Unit::create(BWAPI::Unit bwapiUnit)
{
	using namespace Units;

	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		return new Terran_Marine(bwapiUnit);
	default:
		throw std::runtime_error("unit type "+bwapiUnit->getType().getName()+" not supported yet");
	}
}
