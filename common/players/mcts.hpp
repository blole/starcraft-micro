#pragma once
#include "common/common.hpp"
#include "common/players/player.hpp"
#include "common/actionlisters/actionlister.hpp"
#include "common/selecters/selecter.hpp"
#include "common/stateevaluaters/stateevaluater.hpp"
#include "common/backpropagaters/backpropagater.hpp"
#include "common/terminalcheckers/terminalchecker.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"
#include "common/node.hpp"
#include <chrono>

namespace Bot { namespace Search
{
	namespace Nodes
	{
		struct MCTS : Node<MCTS>
		{
			int visits;
			double totalReward;
		
			MCTS(MCTS* parent, shared_ptr<Effect> effect)
				: Node(parent, std::move(effect))
				, visits(0)
				, totalReward(0)
			{}
		};
	}

	namespace Players
	{
		template<class NT, class ActionListerType, template <class> class SelecterType, class StateEvaluaterType,
				template <class> class BackpropagaterType, class TerminalCheckerType>
		struct MCTS : Player
		{
		private:
			ActionListerType actions;
			SelecterType<NT> select;
			StateEvaluaterType evaluate;
			BackpropagaterType<NT> backpropagate;
			TerminalCheckerType isTerminal;

		public:
			MCTS(
				const ActionListerType&			actionlister	= ActionListerType(),
				const SelecterType<NT>&			selecter		= SelecterType<NT>(),
				const StateEvaluaterType&		evaluater		= StateEvaluaterType(),
				const BackpropagaterType<NT>&	backpropagater	= BackpropagaterType<NT>(),
				const TerminalCheckerType&		isTerminal		= TerminalCheckerType())
				: actions(actionlister)
				, select(selecter)
				, evaluate(evaluater)
				, backpropagate(backpropagater)
				, isTerminal(isTerminal)
			{}

			unique_ptr<NT> buildTree(GameState& rootState)
			{
				using namespace std::chrono;
				auto timeout = steady_clock::now() + std::chrono::milliseconds(40);

				unique_ptr<NT> root = std::make_unique<NT>(nullptr, std::make_shared<NoEffectPlayer<true>>());

				int expansions = 0;
				for (; expansions % 10 != 0 || steady_clock::now() < timeout; expansions++)
				{
					NT* node = root.get();
					GameState state(rootState);

					while (!node->terminal)
					{
						//expansion
						if (node->children.empty())
						{
							if (!isTerminal(state))
							{
								for (shared_ptr<Effect>& effect : actions(state))
									node->children.emplace_back(node, effect);
							}

							if (node->children.empty())
							{
								node->terminal = true;
								break;
							}
						}

						//selection
						node = &select(state, *node);
						state.queueEffect(0, node->effect);
					}

					//simulation
					double score = evaluate(state);

					//backpropagation
					backpropagate(state, *node, score);
				}

				return root;
			}

			vector<shared_ptr<Effect>> operator()(GameState& rootState) override
			{
				unique_ptr<NT> root = buildTree(rootState);
				NT* node = root.get();
				GameState state(rootState);

				vector<shared_ptr<Effect>> bestActions;

				while (state.frame() == 0 && !node->children.empty())
				{
					//TODO: select best, not regular selection
					node = &select(state, *node);
					state.queueEffect(0, node->effect);

					if (node->effect->isPlayerEffect(state))
						bestActions.push_back(node->effect);
				}

				BWAPI::Broodwar->drawTextScreen(200, 70, "taken actions:  %d", bestActions.size());
				BWAPI::Broodwar->drawTextScreen(200, 85, "root.visits:    %d", root->visits);
				BWAPI::Broodwar->drawTextScreen(200, 100, "root.avgReward: %.1f", root->totalReward / root->visits);
				return bestActions;
			}
		};
	}
}}
