#pragma once
#include "common/stateevaluaters/stateevaluater.hpp"
#include "common/terminalcheckers/standard.hpp"
#include "common/behaviortree/behaviortree.hpp"

namespace Bot { namespace StateEvaluaters { namespace Simulaters
{
	template <typename BehaviorTreeType, typename HeuristicType>
	struct SimulaterScripted : Simulater
	{
		using BTT = Behaviors::Root<BehaviorTreeType>;

		const TerminalCheckers::Standard isTerminal;
		const HeuristicType heuristic;

		SimulaterScripted(const HeuristicType heuristic = HeuristicType())
			: heuristic(heuristic)
		{}

		virtual double operator()(GameState& state) const final override
		{
			vector<Behaviors::Root<BehaviorTreeType>> bts;
			for (auto& unit : state.units)
				bts.emplace_back(Behaviors::Root<BehaviorTreeType>());

			while (!isTerminal(state))
			{
				for (int i = 0; i < state.units.size(); i++)
				{
					auto& unit = state.units[i];
					auto& bt = bts[i];

					Effect& action = bt.execute(state, *unit);
					action.applyTo(state);
				}
				state.advanceFrame();
			}
			return heuristic(state);
		}
	};
}}}
