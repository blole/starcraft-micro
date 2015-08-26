#pragma once
#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/effects/effect.hpp"
#include "search/effects/attack.hpp"
#include "search/effects/move.hpp"

namespace Bot { namespace Search
{
	class_Unit(Terran_Marine)
	{
		//typedef ClearAttackFrame <OneUnitEffectData> Attack;
		//typedef ClearAttackFrame <OneUnitEffectData, 7, ClearAttackFrame<OneUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 7, BeginAttack<TwoUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 7, ClearAttackFrame<TwoUnitEffectData>> Attack;
		//typedef BeginAttack <TwoUnitEffectData, 1, ApplyDamage <8, TwoUnitEffectData>> Attack;
		//typedef ApplyDamage <8, 5, ClearAttackFrame<>> Attack;
		//typedef ClearAttackFrame<> Attack;
		typedef BeginAttack<TwoUnitEffectData, 
				1, ApplyDamage<8, TwoUnitEffectData,
				5, ClearAttackFrame<TwoUnitEffectData, 
				10, ClearGroundWeaponCooldown<TwoUnitEffectData
			>>>> Attack;

	public:
		Terran_Marine(BWAPI::Unit bwapiUnit, id_t id)
			: Base(bwapiUnit, id)
		{}

		virtual std::vector<shared_ptr<Effect>> possibleActions(const GameState* state) const override
		{
			static const int range = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange();

			vector<shared_ptr<Effect>> actions;

			if (!groundWeaponCooldown)
			{
				for (auto& unit : state->teamunits(!isPlayerUnit()))
				{
					if (unit->isAlive() && pos.getDistance(unit->pos) <= range)
						actions.push_back(std::make_shared<Attack>(TwoUnitEffectData(id, unit->id)));
				}
			}

			if (!isAttackFrame && !isMoving)
			{
				//TODO: precalc all of these directional offsets..
				for (float dir = 0; dir < 3.14f * 2; dir += 3.15f / 2)
					actions.push_back(std::make_shared<Move>(this, dir));
			}

			return actions;
		}
		
		void firstFrameInitToAddAlreadyActiveEffects(GameState* state) override
		{
			BWAPI::Unit bwapiUnit = getBwapiUnit();

			if (bwapiUnit->isAttackFrame() || bwapiUnit->isStartingAttack() ||
				(BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
				bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit))
			{
				isAttackFrame = true;
				//TODO: set correct move cooldown
				state->queueEffect(6, std::make_shared<ClearAttackFrame<OneUnitEffectData>>(OneUnitEffectData(id)));
			}

			if (bwapiUnit->getGroundWeaponCooldown() != 0)
			{
				groundWeaponCooldown = true;
				state->queueEffect(bwapiUnit->getGroundWeaponCooldown(), std::make_shared<ClearGroundWeaponCooldown<OneUnitEffectData>>(OneUnitEffectData(id)));
			}
			else if (BWAPI::Broodwar->getFrameCount() <= bwapiUnit->getLastCommandFrame() + BWAPI::Broodwar->getRemainingLatencyFrames() &&
				bwapiUnit->getLastCommand().getType() == BWAPI::UnitCommandTypes::Attack_Unit)
			{
				groundWeaponCooldown = true;
				state->queueEffect(14, std::make_shared<ClearGroundWeaponCooldown<OneUnitEffectData>>(OneUnitEffectData(id)));
			}
		}
	};
}}
