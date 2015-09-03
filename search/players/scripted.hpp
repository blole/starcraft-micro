#pragma once
#include "common/common.hpp"
#include "search/players/player.hpp"
#include "search/effects/effect.hpp"
#include "search/behaviors/behaviortreenode.hpp"
#include "search/terminalcheckers/standard.hpp"

namespace Bot { namespace Search { namespace Players
{
	template <class BehaviorTreeType>
	struct Scripted : Player
	{
	private:
		BehaviorTreeType behaviorTree;
		TerminalCheckers::Standard isTerminal;

	public:
		Scripted(const BehaviorTreeType& behaviorTree = BehaviorTreeType())
			: behaviorTree(behaviorTree)
		{}

		vector<shared_ptr<Effect>> operator()(GameState& state) override
		{
			vector<shared_ptr<Effect>> actions;
			if (!isTerminal(state))
			{
				for (auto& unit : state.playerUnits())
					actions.push_back(behaviorTree.execute(state, *unit));
			}
			return actions;
		}
	};
}}}
