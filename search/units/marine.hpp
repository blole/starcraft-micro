#pragma once
#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/effect.hpp"
#include "search/actions/attack.hpp"
#include "search/actions/move.hpp"

namespace Bot { namespace Search
{
	class Terran_Marine : public Unit
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
		Terran_Marine(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
			: Unit(state, bwapiUnit, id)
		{

		}

		virtual std::vector<Effect*> possibleActions(const GameState* state) const
		{
			static const int range = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange();

			std::vector<Effect*> actions;

			if (!groundWeaponCooldown)
			{
				for (const Unit* enemy : state->enemyUnitsInRange(pos, range))
					actions.push_back(new Attack(TwoUnitEffectData(id, enemy->id)));
			}

			if (!isAttackFrame && !isMoving)
			{
				//TODO: precalc all of these directional offsets..
				for (float dir = 0; dir < 3.14f * 2; dir += 3.15f / 2)
					actions.push_back(new Move(this, dir));
			}

			return actions;
		}

		virtual Unit* clone() const
		{
			return new Terran_Marine(*this);
		}
	};
}}