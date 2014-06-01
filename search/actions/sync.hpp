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

		virtual bool applyTo(GameState* state, int frameOffset)
		{
			state->getUnitModifiable(unitID)->groundWeaponCooldown = false;
			return true;
		}

		virtual void executeOrder(GameState* state)
		{}
	};





	class AttackAnimationDone : public SingleUnitAction
	{
	public:
		AttackAnimationDone(const GameState* state, const Unit* unit)
			: SingleUnitAction(state, unit)
		{}

		virtual bool applyTo(GameState* state, int frame)
		{
			state->getUnitModifiable(unitID)->isAttackFrame = false;
			return true;
		}

		virtual void executeOrder(GameState* state)
		{}
	};
}}
