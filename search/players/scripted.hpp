#pragma once
#include "common/common.hpp"
#include "search/players/player.hpp"
#include "search/effects/effect.hpp"
#include "search/behaviors/behaviortree.hpp"
#include "search/terminalcheckers/standard.hpp"

namespace Bot { namespace Search { namespace Players
{
	template <class BehaviorTreeType>
	struct Scripted : Player
	{
	private:
		BehaviorTreeType behaviorTreeTemplate;
		TerminalCheckers::Standard isTerminal;

	public:
		Scripted(const BehaviorTreeType& behaviorTreeTemplate = BehaviorTreeType())
			: behaviorTreeTemplate(behaviorTreeTemplate)
		{}

		vector<shared_ptr<Effect>> operator()(GameState& state) override
		{
			vector<shared_ptr<Effect>> actions;
			if (!isTerminal(state))
			{
				for (auto& unit : state.playerUnits())
				{
					BehaviorTreeType bt(behaviorTreeTemplate);
					actions.push_back(bt.execute(state, *unit));
				}
			}
			return actions;
		}
	};
}}}
