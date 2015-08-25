#pragma once
#include <BWAPI.h>
#include <cassert>

#include "search/searchers/searcher.hpp"
#include "search/actionlisters/actionlister.hpp"
#include "search/selecters/selecter.hpp"
#include "search/simulaters/simulater.hpp"
#include "search/backpropagaters/backpropagater.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include "search/node.hpp"
#include "common/common.hpp"

namespace Bot { namespace Search
{
	template<class NT>
	class NodeMCTS : public Node<NT>
	{
	public:
		int visits;
		double totalReward;
	public:
		NodeMCTS(NT* parent, Effect* effect, int visits = 0, double totalReward = 0)
			: Node(parent, effect)
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
			NodeUCT(NodeUCT* parent, Effect* effect)
				: NodeMCTS(parent, effect)
				, terminal(false)
			{}
		};
	}


	template<class NT>
	class SearcherMCTS : public Searcher
	{
	private:
		shared_ptr<ActionLister> actionlister;
		shared_ptr<Selecter<NT>> selecter;
		shared_ptr<Simulater> simulater;
		shared_ptr<Backpropagater<NT>> backpropagater;
		
	public:
		SearcherMCTS(
			shared_ptr<ActionLister> actionlister,
			shared_ptr<Selecter<NT>> selecter,
			shared_ptr<Simulater> simulater,
			shared_ptr<Backpropagater<NT>> backpropagater)
			: actionlister(actionlister)
			, selecter(selecter)
			, simulater(simulater)
			, backpropagater(backpropagater)
		{}
		
		NT* buildTree(GameState* rootState)
		{
			assert(!rootState->isTerminal());

			NT* root = new NT(nullptr, nullptr);

			//TODO: constrain in time instead
			for (int i = 0; i < 1000; i++)
			{
				NT* node = root;
				GameState state(*rootState);

				while (!node->terminal)
				{
					//expansion
					if (node->children.empty())
					{
						if (!state.isTerminal())
						{
							for (Effect* effect : actionlister->actions(&state))
								node->children.push_back(new NT(node, effect));
						}

						if (node->children.empty())
						{
							node->terminal = true;
							break;
						}
					}

					//selection
					node = selecter->select(node, state);
					state.queueEffect(0, node->effect);
				}

				//simulation
				double score = simulater->simulate(&state);

				//backpropagation
				backpropagater->backpropagate(node, score);
			}

			return root;
		}

		vector<Effect*> search(GameState* rootState) override
		{
			NT* const root = buildTree(rootState);
			NT* node = root;
			GameState state(*rootState);
			
			vector<Effect*> bestActions;

			//TODO: select best, not regular selection
			while (state.getFrame() == 0)
			{
				node = selecter->select(node, state);
				if (node->children.empty())
					break;

				state.queueEffect(0, node->effect);
				
				if (node->effect->isPlayerEffect())
				{
					bestActions.push_back(node->effect);
					node->effect = nullptr; //don't delete these effects when deleting root
				}
			}
			
			BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root->totalReward / root->visits);
			BWAPI::Broodwar->drawTextScreen(0, 30, "root.visits: %d", root->visits);
			delete root;
			return bestActions;
		}
	};
}}
