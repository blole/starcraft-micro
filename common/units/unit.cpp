#include "common/units/unit.hpp"
#include "common/units/marine.hpp"

using namespace Bot;

void Unit::onFrame()
{
	hp_ = bwapiUnit->getHitPoints();
	pos = bwapiUnit->getPosition();
	isMoving = bwapiUnit->isMoving();
	isAttackFrame = bwapiUnit->isAttackFrame();
	groundWeaponCooldown = bwapiUnit->getGroundWeaponCooldown() > 0;

	activeEffects.remove_if([](const FramedEffect& e) {return e.frame < Broodwar->getFrameCount();});
	if (bwapiUnit->isStartingAttack())
	{
		activeEffects.emplace_back(0, make_shared<Effects::BeginAttack<>>(*this, Unit::get(bwapiUnit->getOrderTarget())));
		activeEffects.emplace_back(6, make_shared<Effects::SetAttackFrame<false>>(*this));
		activeEffects.emplace_back(bwapiUnit->getGroundWeaponCooldown(), make_shared<Effects::ClearGroundWeaponCooldown<>>(*this));
	}
}

Unit* Unit::create(BWAPI::Unit bwapiUnit)
{
	using namespace Units;

	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		return new Terran_Marine(bwapiUnit);
	default:
		throw std::runtime_error("only marines supported so far.");
	}
}
