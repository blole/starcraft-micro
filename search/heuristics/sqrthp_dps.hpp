#pragma once
#include "search/gamestate.hpp"
#include "search/unit.hpp"
#include "search/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct SqrtHp_Dps : Heuristic
	{
		virtual double value(const GameState* state) const override
		{
			double sum = 0;

			for (const Unit* unit : state->playerUnits())
			{
				double cd = unit->getBwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->getBwapiUnit()->getType().groundWeapon().damageAmount();
				sum += std::sqrt((double)unit->hp)*dmg*24 / cd;
			}

			for (const Unit* unit : state->enemyUnits())
			{
				double cd = unit->getBwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->getBwapiUnit()->getType().groundWeapon().damageAmount();
				sum -= std::sqrt((double)unit->hp)*dmg*24 / cd;
			}

			return sum;
		}
	};
}}}
