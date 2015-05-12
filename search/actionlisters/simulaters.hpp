#pragma once
#include <BWAPI.h>
#include <vector>
#include "search/gamestate.hpp"
#include "search/actions/effect.hpp"
#include "search/actionlisters/actionlister.hpp"

namespace Bot { namespace Search { 	namespace Simulaters
{
	struct HeuristicFunctionWrapper : public Simulater
	{
		HeuristicFunction* const heuristic;
		HeuristicFunctionWrapper(HeuristicFunction* heuristic)
			: heuristic(heuristic)
		{}
		~HeuristicFunctionWrapper()
		{
			delete heuristic;
		}

		virtual double simulate(GameState* state) const override
		{
			return heuristic->heuristicValue(state);
		}
	};
}}}
