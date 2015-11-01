#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Effects
{
	template<class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class BeginAttack : public TwoUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		BeginAttack(Data data)
			: TwoUnitEffect<Data>(data)
		{}

		BeginAttack(const Unit& unit, const Unit& target)
			: BeginAttack(Data(unit, target))
		{}

		virtual void applyTo(GameState& state) const override
		{
			Unit& unit = state.get(bwapiUnit());
			unit.moveCooldown = 6;
			unit.groundWeaponCooldown = 16;
			queueNext(state, data);
		}
		
		virtual void executeOrder(GameState& state) const override
		{
			Broodwar << Broodwar->getFrameCount() << ": attack order";
			if (bwapiUnit()->getOrderTarget() != bwapiTarget()) //TODO: this line shouldn't be needed
			{
				bwapiUnit()->attack(bwapiTarget());
				Broodwar << " issued";
			}
			Broodwar << endl;
			
			Broodwar->drawLineMap(bwapiUnit()->getPosition(), bwapiTarget()->getPosition(),
				bwapiUnit()->getPlayer() == Broodwar->self() ? BWAPI::Colors::Red : BWAPI::Colors::Blue);
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
			state.get(bwapiTarget()).takeDamage(damage);
			queueNext(state, data);
		}
	};
	
	
	template <int value, class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class SetMoveCooldown : public OneUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		SetMoveCooldown(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			state.get(bwapiUnit()).moveCooldown = value;
			queueNext(state, data);
		}
	};
	
	
	template <int value, class Data = OneUnitEffectData, int offset = 0, class NextEffect = void>
	class SetGroundWeaponCooldown : public OneUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		SetGroundWeaponCooldown(const Data& data)
			: OneUnitEffect(data)
		{}

		virtual void applyTo(GameState& state) const override
		{
			state.get(bwapiUnit()).groundWeaponCooldown = value;
			queueNext(state, data);
		}
	};
}}
