#pragma once
#include "search/simulater.hpp"
#include "search/heuristic.hpp"

namespace Bot { namespace Search { 	namespace Simulaters
{
	struct HeuristicWrapper : public Simulater
	{
		std::shared_ptr<Heuristic> heuristic;
		HeuristicWrapper(std::shared_ptr<Heuristic> heuristic)
			: heuristic(heuristic)
		{}
		
		virtual double simulate(GameState* state) const override
		{
			return heuristic->value(state);
		}
	};
}}}
