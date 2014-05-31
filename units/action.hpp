#pragma once
#include "units/gamestate.hpp"
#include "units/unit.hpp"
#include <BWAPI.h>

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
		bool applyTo(GameState* state) { return applyTo(state, state->getFrame()-frameStarted); }
		virtual bool applyTo(GameState* state, int frame) = 0;
		virtual void executeOrder(GameState* state) = 0;
	};

	class SingleUnitAction : public Action
	{
	protected:
		const id_t unitID;

		SingleUnitAction(GameState* state, Unit* unit)
			: Action(state)
			, unitID(unit->id)
		{}
	};

	class TwoUnitAction : public SingleUnitAction
	{
	protected:
		const id_t targetID;

		TwoUnitAction(GameState* state, Unit* unit, Unit* target)
			: SingleUnitAction(state, unit)
			, targetID(target->id)
		{}
	};





	class Move : public SingleUnitAction
	{
		const BWAPI::Position frameOffset;
	public:
		Move(GameState* state, Unit* unit, float direction)
			: SingleUnitAction(state, unit)
			, frameOffset(
				BWAPI::Position(
					(int)(std::cos(direction)*state->getBwapiUnit(unit)->getType().topSpeed()),
					(int)(std::sin(direction)*state->getBwapiUnit(unit)->getType().topSpeed())))
		{}

		virtual bool applyTo(GameState* state, int frame)
		{
			const Unit* unit = state->getUnit(unitID);
			if (!unit->isAlive() || unit->isAttackFrame)
				return false;

			Unit* unitModifiable = state->getUnitModifiable(unitID);
			unitModifiable->pos += frameOffset;
			state->addEffect(1, this);

			return true;
		}

		virtual void executeOrder(GameState* state)
		{
			state->getBwapiUnit(unitID)->move(frameOffset * 3);
		}
	};

	template<int AttackFrames>
	class Attack : public TwoUnitAction
	{
	public:
		Attack(GameState* state, Unit* unit, Unit* target)
			: TwoUnitAction(state, unit, target)
		{}

		virtual bool applyTo(GameState* state, int frame)
		{
			switch (frame)
			{
			case 1:
				if (state->getUnit(unitID)->isAlive())
				{
					Unit* unit = state->getUnitModifiable(unitID);
					Unit* target = state->getUnitModifiable(targetID);
					unit->isAttackFrame = true;
					unit->groundWeaponCooldown = true;
					target->hp -= state->getBwapiUnit(unitID)->getType().groundWeapon().damageAmount();
					return true;
				}
				else
					return false;
			case AttackFrames:
				Unit* unit = state->getUnitModifiable(unitID);
				unit->isAttackFrame = false;
				return true;
			}
			if (frame == state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown())
			{
				Unit* unit = state->getUnitModifiable(unitID);
				unit->groundWeaponCooldown = false;
				return true;
			}
			return true;
		}

		virtual void executeOrder(GameState* state)
		{
			state->getBwapiUnit(unitID)->attack(state->getBwapiUnit(targetID));
		}
	};

	class GroundWeaponReloaded : public SingleUnitAction
	{
	public:
		GroundWeaponReloaded(GameState* state, Unit* unit)
			: SingleUnitAction(state, unit)
		{}

		virtual bool applyTo(GameState* state, int frame)
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
		AttackAnimationDone(GameState* state, Unit* unit)
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
