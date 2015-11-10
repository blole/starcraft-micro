#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Effects
{
	template<class Data = TwoUnitEffectData, int offset = 0, class NextEffect = void>
	class OrderAttack : public TwoUnitEffect<Data>, public EffectChain<Data, offset, NextEffect>
	{
	public:
		explicit OrderAttack(Data data)
			: TwoUnitEffect<Data>(data)
		{}

		OrderAttack(const Unit& unit, const Unit& target)
			: OrderAttack(Data(unit, target))
		{}

		virtual void applyTo(GameState& state) const override
		{
			Unit& unit = state.get(bwapiUnit());
			unit.moveCooldown = unit.groundWeaponMoveCooldownDefault();
			unit.groundWeaponCooldown = unit.groundWeaponCooldownDefault();
			queueNext(state, data);
		}
		
		virtual void applyPredicted(GameState& state) const override {}
		virtual void applyObserved(GameState& state) const override
		{
			queueNext(state, data);
		}

		virtual void executeOrder(GameState& state) const override
		{
			Broodwar << Broodwar->getFrameCount() << ": attack order";
			if (!(bwapiUnit()->getOrder() == BWAPI::Orders::AttackUnit && 
				  bwapiUnit()->getOrderTarget() == bwapiTarget())) //TODO: this line might not be needed
			{
				//bwapiUnit()->stop(); //this may be an improvement
				bwapiUnit()->attack(bwapiTarget());
				Broodwar << " issued";
			}
			Broodwar << " cd:" << bwapiUnit()->getGroundWeaponCooldown();
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
