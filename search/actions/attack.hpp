#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/actions/effect.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	template<int offset = 0, class NextEffect = void>
	class BeginAttack : public TwoUnitEffect<offset, NextEffect>
	{
	public:
		BeginAttack(const id_t unitID, const id_t targetID)
			: TwoUnitEffect(unitID, targetID)
		{}

		virtual void applyTo(GameState* state) const override
		{
			Unit* unit = state->getUnitModifiable(unitID);
			unit->isAttackFrame = true;
			unit->groundWeaponCooldown = true;
			queueNext(state);
		}
		
		virtual void executeOrder(GameState* state) const override
		{
			BWAPI::Unit unit = state->getBwapiUnit(unitID);
			BWAPI::Unit target = state->getBwapiUnit(targetID);

			if (!unit->isStartingAttack()) //TODO: this line shouldn't be needed
				unit->attack(target);

			BWAPI::Broodwar->drawLineMap(unit->getPosition(), target->getPosition(),
				unit->getPlayer() == BWAPI::Broodwar->self() ?
				BWAPI::Colors::Red : BWAPI::Colors::Blue);
		}
	};
	
	
	template<int damage, int offset = 0, class NextEffect = void>
	class ApplyDamage : public TwoUnitEffect<offset, NextEffect>
	{
	public:
		ApplyDamage(const id_t unitID, const id_t targetID)
			: TwoUnitEffect(unitID, targetID)
		{}
		
		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(targetID)->hp -= damage;
			queueNext(state);
		}
	};
	
	
	template <int offset = 0, class NextEffect = void>
	class ClearAttackFrame : public SingleUnitEffect<offset, NextEffect>
	{
	public:
		ClearAttackFrame(const id_t unitID)
			: SingleUnitEffect(unitID)
		{}

		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(unitID)->isAttackFrame = false;
			queueNext(state);
		}
	};

	
	template <int offset = 0, class NextEffect = void>
	class ClearGroundWeaponCooldown : public SingleUnitEffect<offset, NextEffect>
	{
	public:
		ClearGroundWeaponCooldown(const id_t unitID)
			: SingleUnitEffect(unitID)
		{}
		
		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(unitID)->groundWeaponCooldown = false;
			queueNext(state);
		}
	};
}}
