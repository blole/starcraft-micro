#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct Hp : Heuristic
	{
		virtual double value(const GameState* state) const override
		{
			double sum = 0;

			for (const Unit* unit : state->playerUnits())
				sum += unit->hp;

			for (const Unit* unit : state->enemyUnits())
				sum -= unit->hp;

			return sum;
		}
	};
}}}
