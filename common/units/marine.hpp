#pragma once
#include "common/units/unit.hpp"
#include "common/gamestate.hpp"
#include "common/effects/effect.hpp"
#include "common/effects/attack.hpp"
#include "common/effects/move.hpp"

namespace Bot { namespace Units
{
	struct Terran_Marine : Unit_CRTP<Terran_Marine, BWAPI::UnitTypes::Terran_Marine>
	{
		using Attack = Effects::BeginAttack<TwoUnitEffectData,
				1, Effects::ApplyDamage<8, TwoUnitEffectData,
				5, Effects::SetAttackFrame<false, TwoUnitEffectData,
				10, Effects::SetGroundWeaponCooldown<false, TwoUnitEffectData
				>>>>;

		Terran_Marine(BWAPI::Unit bwapiUnit)
			: Unit_CRTP<Terran_Marine, BWAPI::UnitTypes::Terran_Marine>(bwapiUnit)
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
						actions.push_back(make_shared<Attack>(*this, *unit));
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
	};
}}
