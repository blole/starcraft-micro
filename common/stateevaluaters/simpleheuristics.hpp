#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/stateevaluaters/stateevaluater.hpp"

namespace Bot { namespace StateEvaluaters { namespace Heuristics
{
	struct SimpleHeuristic : Heuristic
	{
		virtual double unitValue(const Unit& unit) const = 0;

		virtual double operator()(GameState& state) const final override
		{
			double sum = 0;
			for (auto& unit : state.playerUnits) sum += unitValue(*unit);
			for (auto& unit : state.enemyUnits)  sum -= unitValue(*unit);
			return sum;
		}
	};

	struct Hp : SimpleHeuristic
	{
		virtual double unitValue(const Unit& unit) const override
		{
			return unit.hp();
		}
	};

	struct SqrtHp : SimpleHeuristic
	{
		virtual double unitValue(const Unit& unit) const override
		{
			return sqrt(unit.hp());
		}
	};

	struct SqrtHp_Dps : SimpleHeuristic
	{
		virtual double unitValue(const Unit& unit) const override
		{
			double cd = unit.bwapiUnit->getType().groundWeapon().damageCooldown();
			double dmg = unit.bwapiUnit->getType().groundWeapon().damageAmount();
			return sqrt(unit.hp()) * dmg*24/cd;
		}
	};
}}}
