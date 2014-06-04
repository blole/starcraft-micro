#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/actions/action.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	template<int AttackFrames> //TODO: weapon cooldown
	class Attack : public TwoUnitAction
	{
	public:
		Attack(const GameState* state, const Unit* unit, const Unit* target)
			: TwoUnitAction(state, unit, target)
		{}

		virtual void applyTo(GameState* state, int frameOffset)
		{
			switch (frameOffset)
			{
			case 0:
				{
					Unit* unit = state->getUnitModifiable(unitID);
					unit->isAttackFrame = true;
					unit->groundWeaponCooldown = true;

					state->enqueueEffect(1, this);
					state->enqueueEffect(AttackFrames, this);
					state->enqueueEffect(state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown(), this);
					return;
				}
			case 1:
				if (state->getUnit(unitID)->isAlive())
				{
					Unit* target = state->getUnitModifiable(targetID);
					target->hp -= state->getBwapiUnit(unitID)->getType().groundWeapon().damageAmount();
				}
				return;
			case AttackFrames:
				Unit* unit = state->getUnitModifiable(unitID);
				unit->isAttackFrame = false;
				return;
			}
			if (frameOffset == state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown())
			{
				Unit* unit = state->getUnitModifiable(unitID);
				unit->groundWeaponCooldown = false;
			}
		}

		virtual void executeOrder(GameState* state)
		{
			state->getBwapiUnit(unitID)->attack(state->getBwapiUnit(targetID));
			BWAPI::Broodwar->drawLineMap(state->getBwapiUnit(unitID)->getPosition(),state->getBwapiUnit(targetID)->getPosition(),BWAPI::Color(255,0,0));
		}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}
	};
}}
