#pragma once
#include "common/common.hpp"
#include "common/players/player.hpp"
#include "common/effects/effect.hpp"
#include "common/behaviortree/behaviortree.hpp"
#include "common/terminalcheckers/standard.hpp"

namespace Bot { namespace Search { namespace Players
{
	template <class BehaviorTreeType>
	struct Scripted : Player
	{
	private:
		const TerminalCheckers::Standard isTerminal;
		const Behaviors::Root<BehaviorTreeType> behaviorTreeTemplate;
		unordered_map<int, Behaviors::Root<BehaviorTreeType>> behaviorTrees;

	public:
		Scripted(const BehaviorTreeType& behaviorTreeTemplate = BehaviorTreeType())
			: behaviorTreeTemplate(behaviorTreeTemplate)
		{}

		vector<shared_ptr<Effect>> operator()(GameState& state) override
		{
			vector<shared_ptr<Effect>> actions;
			if (!isTerminal(state))
			{
				for (auto& unit : state.playerUnits)
				{
					//each Search::Unit has it's own behaviorTree, find it
					int id = unit->bwapiUnit->getID();
					auto iter = behaviorTrees.find(id);
					if (iter == behaviorTrees.end())
						iter = behaviorTrees.emplace(id, behaviorTreeTemplate).first;
					Behaviors::Root<BehaviorTreeType>& unitBehaviorTree = iter->second;

					actions.push_back(unitBehaviorTree.execute(state, *unit));
				}
			}
			return actions;
		}
	};
}}}
