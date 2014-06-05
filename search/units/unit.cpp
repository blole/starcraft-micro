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
	if (bwapiUnit->isAttackFrame())
	{
		isAttackFrame = true;
		//TODO: set correct move cooldown
		state->addEffect(5, new AttackAnimationDone(state, this));
	}
	if (bwapiUnit->getGroundWeaponCooldown() != 0)
	{
		groundWeaponCooldown = true;
		state->addEffect(bwapiUnit->getGroundWeaponCooldown(), new GroundWeaponReloaded(state, this));
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
