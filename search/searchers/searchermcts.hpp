#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searcher.hpp"
#include "search/actionlister.hpp"
#include "search/selecter.hpp"
#include "search/simulater.hpp"
#include "search/backpropagater.hpp"
#include "search/unit.hpp"
#include "search/effect.hpp"
#include "search/node.hpp"
#include <boost/ptr_container/clone_allocator.hpp>

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
		std::shared_ptr<ActionLister> actionlister;
		std::shared_ptr<Selecter<NT>> selecter;
		std::shared_ptr<Simulater> simulater;
		std::shared_ptr<Backpropagater<NT>> backpropagater;
		
	public:
		SearcherMCTS(
			std::shared_ptr<ActionLister> actionlister,
			std::shared_ptr<Selecter<NT>> selecter,
			std::shared_ptr<Simulater> simulater,
			std::shared_ptr<Backpropagater<NT>> backpropagater)
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
								node->children.emplace_back(effect, new NT(node, effect));
						}

						if (node->children.empty())
						{
							node->terminal = true;
							break;
						}
					}

					//selection
					EffectNodePair<NT>& next = selecter->select(node, state);
					state.queueEffect(0, next.effect);
					node = next.node;
				}

				//simulation
				double score = simulater->simulate(&state);

				//backpropagation
				backpropagater->backpropagate(node, score);
			}

			return root;
		}

		std::vector<Effect*> search(GameState* rootState) override
		{
			NT* const root = buildTree(rootState);
			NT* node = root;
			GameState state(*rootState);
			
			std::vector<Effect*> bestActions;

			//TODO: select best, not regular selection
			while (state.getFrame() == 0)
			{
				EffectNodePair<NT>& next = selecter->select(node, state);
				if (next.node == nullptr)
					break;
				if (next.effect->isPlayerEffect())
				{
					bestActions.push_back(next.effect);
					next.effect = nullptr;
				}
				state.queueEffect(0, next.effect);
				node = next.node;
			}
			
			BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root->totalReward / root->visits);
			BWAPI::Broodwar->drawTextScreen(0, 30, "root.visits: %d", root->visits);
			delete root;
			return bestActions;
		}
	};
}}
