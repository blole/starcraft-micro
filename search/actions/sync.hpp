#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/actions/action.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	class GroundWeaponReloaded : public SingleUnitAction
	{
	public:
		GroundWeaponReloaded(const GameState* state, const Unit* unit)
			: SingleUnitAction(state, unit)
		{}

		virtual void applyTo(GameState* state, int frameOffset)
		{
			state->getUnitModifiable(unitID)->groundWeaponCooldown = false;
		}

		virtual void executeOrder(GameState* state)
		{}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}

		virtual GroundWeaponReloaded* clone() const
		{
			return new GroundWeaponReloaded(*this);
		}
	};





	class AttackAnimationDone : public SingleUnitAction
	{
	public:
		AttackAnimationDone(const GameState* state, const Unit* unit)
			: SingleUnitAction(state, unit)
		{}

		virtual void applyTo(GameState* state, int frame)
		{
			state->getUnitModifiable(unitID)->isAttackFrame = false;
		}

		virtual void executeOrder(GameState* state)
		{}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}

		virtual AttackAnimationDone* clone() const
		{
			return new AttackAnimationDone(*this);
		}
	};
}}
