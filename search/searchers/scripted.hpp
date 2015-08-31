#pragma once
#include "common/common.hpp"
#include "search/searchers/searcher.hpp"
#include "search/effects/effect.hpp"
#include "search/behaviors/behaviortreenode.hpp"

namespace Bot { namespace Search
{
	class Scripted : public Searcher
	{
	private:
		shared_ptr<BehaviorTreeNode> behaviorTree;

	public:
		Scripted(shared_ptr<BehaviorTreeNode> behaviorTree)
			: behaviorTree(behaviorTree)
		{}

		vector<shared_ptr<Effect>> operator()(GameState& state) override
		{
			vector<shared_ptr<Effect>> actions;
			for (auto& unit : state.playerUnits())
				actions.push_back(behaviorTree->execute(state, *unit));
			return actions;
		}
	};
}}
