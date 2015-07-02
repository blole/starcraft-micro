#pragma once
#include "search/simulater.hpp"
#include "search/heuristic.hpp"

namespace Bot { namespace Search { 	namespace Simulaters
{
	struct HeuristicWrapper : public Simulater
	{
		Heuristic* const heuristic;
		HeuristicWrapper(Heuristic* heuristic)
			: heuristic(heuristic)
		{}
		~HeuristicWrapper()
		{
			delete heuristic;
		}

		virtual double simulate(GameState* state) const override
		{
			return heuristic->value(state);
		}
	};
}}}
