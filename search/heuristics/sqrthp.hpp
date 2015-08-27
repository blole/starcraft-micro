#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { namespace Heuristics
{
	struct SqrtHp : Heuristic
	{
		virtual double value(const GameState& state) const override
		{
			double sum = 0;

			for (auto& unit : state.playerUnits())
				sum += sqrt((double)unit->hp);

			for (auto& unit : state.enemyUnits())
				sum -= sqrt((double)unit->hp);

			return sum;
		}
	};
}}}
