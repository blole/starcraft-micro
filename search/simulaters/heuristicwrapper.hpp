#pragma once
#include "common/common.hpp"
#include "search/simulaters/simulater.hpp"
#include "search/heuristics/heuristic.hpp"

namespace Bot { namespace Search { 	namespace Simulaters
{
	struct HeuristicWrapper : public Simulater
	{
		shared_ptr<Heuristic> heuristic;
		HeuristicWrapper(const shared_ptr<Heuristic>& heuristic)
			: heuristic(heuristic)
		{}
		
		virtual double operator()(GameState& state) const override
		{
			return (*heuristic)(state);
		}
	};
}}}
