#pragma once
#include "common/units/unit.hpp"
#include "common/gamestate.hpp"
#include "common/effects/effect.hpp"
#include "common/effects/attack.hpp"
#include "common/effects/move.hpp"

namespace Bot { namespace Search
{
	struct Terran_Marine : Unit_CRTP<Terran_Marine>
	{
		//typedef SetAttackFrame <OneUnitEffectData> Attack;
		//typedef SetAttackFrame <OneUnitEffectData, 7, SetAttackFrame<OneUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 7, BeginAttack<TwoUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 7, SetAttackFrame<TwoUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 1, ApplyDamage <8, TwoUnitEffectData>> Attack;
		//typedef ApplyDamage <8, 5, SetAttackFrame<>> Attack;
		//typedef SetAttackFrame<> Attack;
		typedef Effects::BeginAttack<TwoUnitEffectData, 
				1, Effects::ApplyDamage<8, TwoUnitEffectData,
				5, Effects::SetAttackFrame<false, TwoUnitEffectData,
				10, Effects::ClearGroundWeaponCooldown<TwoUnitEffectData
			>>>> Attack;

	public:
		Terran_Marine(BWAPI::Unit bwapiUnit)
			: Unit_CRTP<Terran_Marine>(bwapiUnit)
		{}

		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const override
		{
			static const int range = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange();

			vector<shared_ptr<Effect>> actions;

			if (!groundWeaponCooldown)
			{
				for (auto& unit : state.teamunits(!isPlayer))
				{
					if (unit->isAlive() && pos.getDistance(unit->pos) <= range)
						actions.push_back(make_shared<Attack>(TwoUnitEffectData(*this, *unit)));
				}
			}

			if (!isAttackFrame && !isMoving)
			{
				//TODO: precalc all of these directional offsets..
				for (float dir = 0; dir < 3.14f * 2; dir += 3.15f / 2)
					actions.push_back(make_shared<Effects::Move<>>(*this, dir));
			}

			return actions;
		}
		
		void firstFrameInitToAddAlreadyActiveEffects(GameState& state) override
		{
			if (bwapiUnit->isAttackFrame())// ||
				//(BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
				//bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit))
			{
				isAttackFrame = true;
				//TODO: set correct move cooldown
				state.queueEffect(6, make_shared<Effects::SetAttackFrame<false>>(*this));
			}

			if (bwapiUnit->getGroundWeaponCooldown() != 0)
			{
				groundWeaponCooldown = true;
				state.queueEffect(bwapiUnit->getGroundWeaponCooldown(), make_shared<Effects::ClearGroundWeaponCooldown<>>(*this));
			}
			else if (BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
				bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit)
			{
				groundWeaponCooldown = true;
				state.queueEffect(14, make_shared<Effects::ClearGroundWeaponCooldown<>>(*this));
			}
		}
	};
}}
