#include "units/unit.hpp"
#include "units/action.hpp"
#include "units/gamestate.hpp"

namespace Bot { namespace Units {

Unit::Unit(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
	: id(id)
	, hp(bwapiUnit->getHitPoints())
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

class Terran_Marine : public Unit
{
public:
	Terran_Marine(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
		: Unit(state, bwapiUnit, id)
	{

	}

	virtual std::list<Action*> const possibleActions(GameState* state)
	{
		static const int range = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange();

		std::list<Action*> actions;

		for each (Unit* unit in state->unitsInRange(pos, range))
			actions.push_back(new Attack(state, this, unit));

		for (float dir = 0; dir < 3.14f * 2; dir += 3.15f / 4)
			actions.push_back(new Move(state, this, dir));

		return actions;
	}

	virtual Unit* const clone()
	{
		return new Terran_Marine(*this);
	}
};



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


}}