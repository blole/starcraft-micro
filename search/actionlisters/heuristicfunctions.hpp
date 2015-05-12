#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/actionlisters/actionlister.hpp"

namespace Bot { namespace Search { namespace HeuristicFunctions
{
	struct Hp : HeuristicFunction
	{
		double heuristicValue(const GameState* state) const
		{
			double sum = 0;

			for (const Unit* unit : state->playerUnits())
				sum += unit->hp;

			for (const Unit* unit : state->enemyUnits())
				sum -= unit->hp;

			return sum;
		}
	};
	struct SqrtHp : HeuristicFunction
	{
		double heuristicValue(const GameState* state) const
		{
			double sum = 0;

			for (const Unit* unit : state->playerUnits())
				sum += std::sqrt((double)unit->hp);

			for (const Unit* unit : state->enemyUnits())
				sum -= std::sqrt((double)unit->hp);

			return sum;
		}
	};
	struct SqrtHp_Dps : HeuristicFunction
	{
		double heuristicValue(const GameState* state) const
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
