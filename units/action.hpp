#pragma once
#include "units/gamestate.hpp"
#include "units/unit.hpp"

namespace Bot { namespace Units
{
	class Action
	{
	protected:
		const int frameStarted;

		Action(GameState* state)
			: frameStarted(state->getFrame())
		{}

	public:
		virtual bool applyTo(GameState* state);
	};

	class SingleUnitEffect : public Action
	{
	protected:
		const id_t unitID;

		SingleUnitEffect(GameState* state, Unit* unit)
			: Action(state)
			, unitID(unit->id)
		{}
	};

	class TwoUnitEffect : public SingleUnitEffect
	{
	protected:
		const id_t targetID;

		TwoUnitEffect(GameState* state, Unit* unit, Unit* target)
			: SingleUnitEffect(state, unit)
			, targetID(target->id)
		{}
	};

	class Move : public SingleUnitEffect
	{
		const BWAPI::Position frameOffset;
	public:
		Move(GameState* state, Unit* unit, float direction)
			: SingleUnitEffect(state, unit)
			, frameOffset(
				BWAPI::Position(
					(int)(std::cos(direction)*state->getBwapiUnit(unit)->getType().topSpeed()),
					(int)(std::sin(direction)*state->getBwapiUnit(unit)->getType().topSpeed())))
		{}

		virtual bool applyTo(GameState* state)
		{
			const Unit* unit = state->getUnit(unitID);
			if (!unit->isAlive() || unit->isAttackFrame)
				return false;

			Unit* unitModifiable = state->getUnitModifiable(unitID);
			unitModifiable->pos += frameOffset;
			state->addEffect(1, this);

			return true;
		}
	};

	//template<int AttackFrames, int ReloadFrames>
	class Attack : public TwoUnitEffect
	{
	public:
		Attack(GameState* state, Unit* unit, Unit* target)
			: TwoUnitEffect(state, unit, target)
		{}
	};

	class GroundWeaponReloaded : public SingleUnitEffect
	{
	public:
		GroundWeaponReloaded(GameState* state, Unit* unit)
			: SingleUnitEffect(state, unit)
		{}

		virtual bool applyTo(GameState* state)
		{
			state->getUnitModifiable(unitID)->groundWeaponCooldown = false;
			return true;
		}
	};

	class AttackAnimationDone : public SingleUnitEffect
	{
	public:
		AttackAnimationDone(GameState* state, Unit* unit)
			: SingleUnitEffect(state, unit)
		{}

		virtual bool applyTo(GameState* state)
		{
			state->getUnitModifiable(unitID)->isAttackFrame = false;
			return true;
		}
	};
}}
