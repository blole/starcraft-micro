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
#include <boost/format.hpp>

namespace Bot
{
	namespace Nodes
	{
		struct MCTS : Node<MCTS>
		{
			unsigned int visits;
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
			unsigned int actioncount = 0;
			unsigned int actioncountframeadvance = 0;

		public:
			explicit MCTS(
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

				unique_ptr<NT> root = std::make_unique<NT>(nullptr, std::make_shared<Effects::NoEffectPlayer<true>>());

				int expansions = 0;
				for (; expansions % 10 != 0 || steady_clock::now() < timeout; expansions++)
				{
					NT* node = root.get();
					GameState state(rootState);

					while (!node->terminal)
					{
						//expansion/termination
						if (!node->expanded())
						{
							if (isTerminal(state)) //termination
							{
								node->terminal = true;
								break;
							}
							else //expansion
							{
								auto allActions = actions(state);
								if (!allActions.empty())
								{
									for (shared_ptr<Effect>& effect : allActions)
										node->children.emplace_back(node, effect);
									actioncount+=allActions.size();
								}
								else
								{
									node->children.emplace_back(node, make_shared<Effects::AdvanceFrameEffect>());
									actioncountframeadvance++;
								}
							}
						}

						//always select unvisited children first
						if (node->visits < node->children.size())
						{
							node = &node->children[node->visits];
							state.queueEffect(0, node->effect);
							goto simulate;
						}

						//selection
						node = &select(state, *node);
						state.queueEffect(0, node->effect);
					}
					simulate:

					//simulation
					double score = evaluate(state);

					//backpropagation
					backpropagate(state, *node, score);
				}

				return root;
			}

			void printTree(NT& n, string prefix, int depth)
			{
				static auto f = boost::format("%-10s:%.2f/%d=%.2f, ucb:%.2f");
				Broodwar << f % prefix.c_str() % n.totalReward % n.visits % (n.totalReward / n.visits) % (n.parent?Selecters::UCB<NT>::ucb(n):-1);

				Broodwar << " children:" << n.children.size();

				if (n.terminal)
					Broodwar << " terminal";

				Broodwar << endl;

				if (depth > 0)
				{
					for (unsigned int i = 0; i < n.children.size(); i++)
						printTree(n.children[i], prefix + " " + std::to_string(i), depth - 1);
				}
			}

			void paintTree(NT& n, GameState state)
			{
				state.queueEffect(0, n.effect);
				for (auto& u : state.units)
				{
					Broodwar->drawCircleMap(u->pos, 0, BWAPI::Colors::Yellow, true);
				}
				for (NT& child : n.children)
					paintTree(child, GameState(state));
			}

			vector<shared_ptr<Effect>> operator()(GameState& rootState) override
			{
				actioncount = 0;
				actioncountframeadvance = 0;
				vector<shared_ptr<Effect>> bestActions;
				unique_ptr<NT> root = buildTree(rootState);
				NT* node = root.get();
				GameState state(rootState);

				//root->effect is just a NoEffect, so don't push it to bestActions

				while (node->expanded())
				{
					//TODO: select best, not regular selection?
					node = &select(state, *node);
					state.queueEffect(0, node->effect);

					if (state.frame() != rootState.frame()) //AdvanceFrameEffect
						break;

					bestActions.push_back(node->effect);
				}

				Broodwar << Broodwar->getFrameCount() << endl;
				printTree(*root, "", 2);
				paintTree(*root, GameState(rootState));

				Broodwar->drawTextScreen(200, 70, "taken actions:  %d", bestActions.size());
				Broodwar->drawTextScreen(200, 85, "root.visits:    %d", root->visits);
				Broodwar->drawTextScreen(200, 100, "root.avgReward: %.1f", root->totalReward / root->visits);

				Broodwar->drawTextScreen(565, 300, "actions: %d", actioncount);
				Broodwar->drawTextScreen(530, 315, "advanceframe: %d", actioncountframeadvance);
				return bestActions;
			}
		};
	}
}
