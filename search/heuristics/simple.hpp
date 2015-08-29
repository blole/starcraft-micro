#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct SimpleHeuristic : Heuristic
	{
		virtual double unitValue(const unique_ptr<Unit>& unit) const = 0;

		virtual double operator()(const GameState& state) const override
		{
			double sum = 0;
			for (auto& unit : state.playerUnits()) sum += unitValue(unit);
			for (auto& unit : state.enemyUnits())  sum -= unitValue(unit);
			return sum;
		}
	};

	struct Hp : public SimpleHeuristic
	{
		virtual double unitValue(const unique_ptr<Unit>& unit) const override
		{
			return unit->hp();
		}
	};

	struct SqrtHp : public SimpleHeuristic
	{
		virtual double unitValue(const unique_ptr<Unit>& unit) const override
		{
			return sqrt(unit->hp());
		}
	};

	struct SqrtHp_Dps : public SimpleHeuristic
	{
		virtual double unitValue(const unique_ptr<Unit>& unit) const override
		{
			double cd = unit->bwapiUnit->getType().groundWeapon().damageCooldown();
			double dmg = unit->bwapiUnit->getType().groundWeapon().damageAmount();
			return sqrt(unit->hp()) * dmg*24/cd;
		}
	};
}}}