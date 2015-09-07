#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Search { namespace Effects
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

			Broodwar << Broodwar->getFrameCount() << ": attack order";
			if (unit->getOrderTarget() != target) //TODO: this line shouldn't be needed
			{
				unit->attack(target);
				Broodwar << " issued";
			}
			Broodwar << endl;
			
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
			state.units[targetID()]->takeDamage(damage);
			queueNext(state, data);
		}
	};
	
	
	template <bool value, class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class SetAttackFrame : public OneUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		SetAttackFrame(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			state.units[unitID()]->isAttackFrame = value;
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
}}}
