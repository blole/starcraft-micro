#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	template<class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class BeginAttack : public TwoUnitEffect<Data>, public EffectChain<offset, NextEffect>
	{
	public:
		BeginAttack(Data data)
			: TwoUnitEffect<Data>(data)
		{}

		virtual void applyTo(GameState* state) const override
		{
			Unit* unit = state->getUnitModifiable(unitID());
			unit->isAttackFrame = true;
			unit->groundWeaponCooldown = true;
			queueNext(state, data);
		}
		
		virtual void executeOrder(GameState* state) const override
		{
			BWAPI::Unit unit = state->getBwapiUnit(unitID());
			BWAPI::Unit target = state->getBwapiUnit(targetID());

			if (!unit->isStartingAttack()) //TODO: this line shouldn't be needed
				unit->attack(target);

			BWAPI::Broodwar->drawLineMap(unit->getPosition(), target->getPosition(),
				unit->getPlayer() == BWAPI::Broodwar->self() ?
				BWAPI::Colors::Red : BWAPI::Colors::Blue);
		}
	};
	
	
	template <int damage, class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class ApplyDamage : public TwoUnitEffect<Data>, public EffectChain<offset, NextEffect>
	{
	public:
		ApplyDamage(const Data& data)
			: TwoUnitEffect(data)
		{}
		
		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(targetID())->hp -= damage;
			queueNext(state, data);
		}
	};
	
	
	template <class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class ClearAttackFrame : public OneUnitEffect<Data>, public EffectChain<offset, NextEffect>
	{
	public:
		ClearAttackFrame(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(unitID())->isAttackFrame = false;
			queueNext(state, data);
		}
	};
	
	
	template <class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class ClearGroundWeaponCooldown : public OneUnitEffect<Data>, public EffectChain<offset, NextEffect>
	{
	public:
		ClearGroundWeaponCooldown(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState* state) const override
		{
			state->getUnitModifiable(unitID())->groundWeaponCooldown = false;
			queueNext(state, data);
		}
	};
}}