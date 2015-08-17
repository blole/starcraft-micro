#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct SqrtHp : Heuristic
	{
		virtual double value(const GameState* state) const override
		{
			double sum = 0;

			for (const Unit* unit : state->playerUnits())
				sum += std::sqrt((double)unit->hp);

			for (const Unit* unit : state->enemyUnits())
				sum -= std::sqrt((double)unit->hp);

			return sum;
		}
	};
}}}
