#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/attack.hpp"
#include "search/units/marine.hpp"

using namespace Bot::Search;

Unit::Unit(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
	: id(id)
	, hp(bwapiUnit->getHitPoints())
	, isMoving(false)
	, isAttackFrame(false)
	, groundWeaponCooldown(false)
{
	if (bwapiUnit->isAttackFrame() || bwapiUnit->isStartingAttack() ||
		(BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
		 bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit))
	{
		isAttackFrame = true;
		//TODO: set correct move cooldown
		state->queueEffect(6, new ClearAttackFrame<OneUnitEffectData>(OneUnitEffectData(id)));
	}

	if (bwapiUnit->getGroundWeaponCooldown() != 0)
	{
		groundWeaponCooldown = true;
		state->queueEffect(bwapiUnit->getGroundWeaponCooldown(), new ClearGroundWeaponCooldown<OneUnitEffectData>(OneUnitEffectData(id)));
	}
	else if (BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
			 bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit)
	{
		groundWeaponCooldown = true;
		state->queueEffect(14, new ClearGroundWeaponCooldown<OneUnitEffectData>(OneUnitEffectData(id)));
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
