#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct SqrtHp_Dps : Heuristic
	{
		virtual double operator()(const GameState& state) const override
		{
			static auto sqrthp_dps = [](const unique_ptr<Unit>& unit)
			{
				double cd = unit->bwapiUnit->getType().groundWeapon().damageCooldown();
				double dmg = unit->bwapiUnit->getType().groundWeapon().damageAmount();
				return sqrt(unit->hp()) * dmg*24/cd;
			};

			double sum = 0;

			for (auto& unit : state.playerUnits())
				sum += sqrthp_dps(unit);

			for (auto& unit : state.enemyUnits())
				sum -= sqrthp_dps(unit);

			return sum;
		}
	};
}}}
