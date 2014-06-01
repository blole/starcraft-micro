#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	class Action
	{
	protected:
		const int frameStarted;

		Action(const GameState* state)
			: frameStarted(state->getFrame())
		{}

	public:
		bool applyTo(GameState* state) { return applyTo(state, state->getFrame()-frameStarted); }
		virtual bool applyTo(GameState* state, int frameOffset) = 0;
		virtual void executeOrder(GameState* state) = 0;
	};

	class SingleUnitAction : public Action
	{
	protected:
		const id_t unitID;

		SingleUnitAction(const GameState* state, const Unit* unit)
			: Action(state)
			, unitID(unit->id)
		{}
	};

	class TwoUnitAction : public SingleUnitAction
	{
	protected:
		const id_t targetID;

		TwoUnitAction(const GameState* state, const Unit* unit, const Unit* target)
			: SingleUnitAction(state, unit)
			, targetID(target->id)
		{}
	};




	class Move : public SingleUnitAction
	{
		static const int moveQuanta = 25;
		const BWAPI::Position moveOffset;
	public:
		Move(const GameState* state, const Unit* unit, float direction)
			: SingleUnitAction(state, unit)
			, moveOffset(
				BWAPI::Position(
					(int)(std::cos(direction)*state->getBwapiUnit(unit)->getType().topSpeed()),
					(int)(std::sin(direction)*state->getBwapiUnit(unit)->getType().topSpeed())))
		{}

		virtual bool applyTo(GameState* state, int frameOffset)
		{
			Unit* unit = state->getUnitModifiable(unitID);
			if (!unit->isAlive() || unit->isAttackFrame || frameOffset >= moveQuanta)
			{
				unit->isMoving = false;
				return false;
			}
			else
			{
				state->addEffect(1, this);

				if (frameOffset == 0)
					unit->isMoving = true;
				else
					unit->pos += moveOffset;

				return true;
			}
		}

		virtual void executeOrder(GameState* state)
		{
			state->getBwapiUnit(unitID)->move(moveOffset * 3);
		}
	};

	template<int AttackFrames> //TODO: weapon cooldown
	class Attack : public TwoUnitAction
	{
	public:
		Attack(const GameState* state, const Unit* unit, const Unit* target)
			: TwoUnitAction(state, unit, target)
		{}

		virtual bool applyTo(GameState* state, int frameOffset)
		{
			switch (frameOffset)
			{
			case 0:
				{
					Unit* unit = state->getUnitModifiable(unitID);
					unit->isAttackFrame = true;
					unit->groundWeaponCooldown = true;

					state->addEffect(1, this);
					state->addEffect(AttackFrames, this);
					state->addEffect(state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown(), this);
					break;
				}
			case 1:
				if (state->getUnit(unitID)->isAlive())
				{
					Unit* target = state->getUnitModifiable(targetID);
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
			if (frameOffset == state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown())
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
