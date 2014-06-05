#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/sync.hpp"
#include "search/units/marine.hpp"

using namespace Bot::Search;

Unit::Unit(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
	: id(id)
	, hp(bwapiUnit->getHitPoints())
	, isMoving(false)
	, isAttackFrame(false)
	, groundWeaponCooldown(false)
{
	if (bwapiUnit->isAttackFrame() ||
		(BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
		 bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit))
	{
		isAttackFrame = true;
		//TODO: set correct move cooldown
		state->addEffect(6, new AttackAnimationDone(state, this));
	}

	if (bwapiUnit->getGroundWeaponCooldown() != 0)
	{
		groundWeaponCooldown = true;
		state->addEffect(bwapiUnit->getGroundWeaponCooldown(), new GroundWeaponReloaded(state, this));
	}
	else if (BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
			 bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit)
	{
		groundWeaponCooldown = true;
		state->addEffect(14, new GroundWeaponReloaded(state, this));
	}
}

Unit* Unit::create(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
{
	switch (bwapiUnit->getType().getID())
	{
	case BWAPI::UnitTypes::Enum::Terran_Marine:
		return new Terran_Marine(state, bwapiUnit, id);
	default:
		throw std::runtime_error("only marines supported for MCTS so far.");
	}
}
