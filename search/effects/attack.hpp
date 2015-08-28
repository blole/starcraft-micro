#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	template<class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class BeginAttack : public TwoUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		BeginAttack(Data data)
			: TwoUnitEffect<Data>(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			auto& unit = state.units[unitID()];
			unit->isAttackFrame = true;
			unit->groundWeaponCooldown = true;
			queueNext(state, data);
		}
		
		virtual void executeOrder(GameState& state) const override
		{
			BWAPI::Unit unit = state.units[unitID()]->bwapiUnit;
			BWAPI::Unit target = state.units[targetID()]->bwapiUnit;

			if (!unit->isStartingAttack()) //TODO: this line shouldn't be needed
				unit->attack(target);

			BWAPI::Broodwar->drawLineMap(unit->getPosition(), target->getPosition(),
				unit->getPlayer() == BWAPI::Broodwar->self() ?
				BWAPI::Colors::Red : BWAPI::Colors::Blue);
		}
	};
	
	
	template <int damage, class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class ApplyDamage : public TwoUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		ApplyDamage(const Data& data)
			: TwoUnitEffect(data)
		{}
		
		virtual void applyTo(GameState& state) const override
		{
			state.units[targetID()]->hp -= damage;
			queueNext(state, data);
		}
	};
	
	
	template <class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class ClearAttackFrame : public OneUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		ClearAttackFrame(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			state.units[unitID()]->isAttackFrame = false;
			queueNext(state, data);
		}
	};
	
	
	template <class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class ClearGroundWeaponCooldown : public OneUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		ClearGroundWeaponCooldown(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			state.units[unitID()]->groundWeaponCooldown = false;
			queueNext(state, data);
		}
	};
}}
