#pragma once
#include "common/common.hpp"
#include "search/searchers/searcher.hpp"
#include "search/actionlisters/actionlister.hpp"
#include "search/selecters/selecter.hpp"
#include "search/simulaters/simulater.hpp"
#include "search/backpropagaters/backpropagater.hpp"
#include "search/terminalcheckers/terminalchecker.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include "search/node.hpp"
#include <chrono>

namespace Bot { namespace Search
{
	template<class NT>
	class NodeMCTS : public Node<NT>
	{
	public:
		int visits;
		double totalReward;
	public:
		NodeMCTS(NT* parent, shared_ptr<Effect> effect, int visits = 0, double totalReward = 0)
			: Node(parent, std::move(effect))
			, visits(visits)
			, totalReward(totalReward)
		{}
	};


	namespace UCT
	{
		class NodeUCT : public NodeMCTS<NodeUCT>
		{
		public:
			bool terminal;
		public:
			NodeUCT(NodeUCT* parent, shared_ptr<Effect> effect)
				: NodeMCTS(parent, std::move(effect))
				, terminal(false)
			{}
		};
	}


	template<class NT>
	class SearcherMCTS : public Searcher
	{
	private:
		shared_ptr<ActionLister> actions;
		shared_ptr<Selecter<NT>> select;
		shared_ptr<Simulater> simulate;
		shared_ptr<Backpropagater<NT>> backpropagate;
		shared_ptr<TerminalChecker> isTerminal;

	public:
		SearcherMCTS(
			shared_ptr<ActionLister> actionlister,
			shared_ptr<Selecter<NT>> selecter,
			shared_ptr<Simulater> simulater,
			shared_ptr<Backpropagater<NT>> backpropagater,
			shared_ptr<TerminalChecker> isTerminal)
			: actions(actionlister)
			, select(selecter)
			, simulate(simulater)
			, backpropagate(backpropagater)
			, isTerminal(isTerminal)
		{}
		
		unique_ptr<NT> buildTree(GameState& rootState)
		{
			using namespace std::chrono;
			auto timeout = steady_clock::now()+std::chrono::milliseconds(40);

			unique_ptr<NT> root = std::make_unique<NT>(nullptr, std::make_shared<NoEffectPlayer<true>>());

			int expansions = 0;
			for (; expansions%10!=0 || steady_clock::now() < timeout; expansions++)
			{
				NT* node = root.get();
				GameState state(rootState);

				while (!node->terminal)
				{
					//expansion
					if (node->children.empty())
					{
						if (!(*isTerminal)(state))
						{
							for (shared_ptr<Effect>& effect : (*actions)(state))
								node->children.push_back(std::make_unique<NT>(node, effect));
						}

						if (node->children.empty())
						{
							node->terminal = true;
							break;
						}
					}

					//selection
					node = (*select)(node, state);
					state.queueEffect(0, node->effect);
				}

				//simulation
				double score = (*simulate)(state);

				//backpropagation
				(*backpropagate)(state, node, score);
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
				node = (*select)(node, state);
				state.queueEffect(0, node->effect);
				
				if (node->effect->isPlayerEffect(state))
					bestActions.push_back(node->effect);
			}
			
			BWAPI::Broodwar->drawTextScreen(200, 70,  "taken actions:  %d", bestActions.size());
			BWAPI::Broodwar->drawTextScreen(200, 85,  "root.visits:    %d", root->visits);
			BWAPI::Broodwar->drawTextScreen(200, 100, "root.avgReward: %.1f", root->totalReward / root->visits);
			return bestActions;
		}
	};
}}
