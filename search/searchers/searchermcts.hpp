#pragma once
#include "common/common.hpp"
#include "search/searchers/searcher.hpp"
#include "search/actionlisters/actionlister.hpp"
#include "search/selecters/selecter.hpp"
#include "search/simulaters/simulater.hpp"
#include "search/backpropagaters/backpropagater.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include "search/node.hpp"

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
		
	public:
		SearcherMCTS(
			shared_ptr<ActionLister> actionlister,
			shared_ptr<Selecter<NT>> selecter,
			shared_ptr<Simulater> simulater,
			shared_ptr<Backpropagater<NT>> backpropagater)
			: actions(actionlister)
			, select(selecter)
			, simulate(simulater)
			, backpropagate(backpropagater)
		{}
		
		unique_ptr<NT> buildTree(GameState& rootState)
		{
			assert(!rootState.isTerminal());

			unique_ptr<NT> root = std::make_unique<NT>(nullptr, nullptr);

			//TODO: constrain in time instead
			for (int i = 0; i < 100; i++)
			{
				NT* node = root.get();
				GameState state(rootState);

				while (!node->terminal)
				{
					//expansion
					if (node->children.empty())
					{
						if (!state.isTerminal())
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

			//TODO: select best, not regular selection
			while (state.getFrame() == 0 && !node->children.empty())
			{
				node = (*select)(node, state);
				state.queueEffect(0, node->effect);
				
				if (node->effect->isPlayerEffect(state))
					bestActions.push_back(node->effect);
			}
			
			BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root->totalReward / root->visits);
			BWAPI::Broodwar->drawTextScreen(0, 30, "root.visits: %d", root->visits);
			return bestActions;
		}
	};
}}
