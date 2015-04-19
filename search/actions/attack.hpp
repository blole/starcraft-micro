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

					state->addEffect(1, this);
					state->addEffect(AttackFrames, this);
					state->addEffect(state->getBwapiUnit(unitID)->getType().groundWeapon().damageCooldown(), this);
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
			BWAPI::Unit unit = state->getBwapiUnit(unitID);
			BWAPI::Unit target = state->getBwapiUnit(targetID);
			
			if (!unit->isStartingAttack()) //TODO: this line shouldn't be needed
				unit->attack(target);

			BWAPI::Broodwar->drawLineMap(unit->getPosition(), target->getPosition(),
				unit->getPlayer() == BWAPI::Broodwar->self() ?
					BWAPI::Colors::Red : BWAPI::Colors::Blue);
		}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}

		virtual Attack* clone() const
		{
			return new Attack(*this);
		}
	};
}}
