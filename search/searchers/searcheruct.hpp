#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searchers/searcher.hpp"
#include "search/units/unit.hpp"
#include "search/actions/effect.hpp"
#include <boost/ptr_container/clone_allocator.hpp>

namespace Bot { namespace Search
{
	template<class NT>
	class NodeMCTS : public Node<NT>
	{
	public:
		int visits;
		double totalReward;
		NodeMCTS(NT* parent, int visits = 0, double totalReward = 0)
			: Node(parent)
			, visits(visits)
			, totalReward(totalReward)
		{}
	};


	template<class NT>
	struct NodeStatePair
	{
		NT& node;
		GameState& state;

		NodeStatePair(NT& node, GameState& state)
			: node(node)
			, state(state)
		{}
	};

	namespace UCT
	{
		class NodeUCT : public NodeMCTS<NodeUCT>
		{
		public:
			bool fullyExpanded;
			bool terminal;

		public:
			NodeUCT(NodeUCT* const parent)
				: NodeMCTS(parent)
				, fullyExpanded(false)
				, terminal(false)
			{
				std::vector<Effect*> actions;
			}

		public:
		};
	}


	template<class NT>
	class SearcherMCTS : public Searcher
	{
	private:
		ActionLister* actionlister;
		Selecter<NT>* selecter;
		Simulater* simulater;
		Backpropagater<NT>* backpropagater;

	public:
		SearcherMCTS(ActionLister* actionlister, Selecter<NT>* selecter, Simulater* simulater, Backpropagater<NT>* backpropagater)
			: actionlister(actionlister)
			, selecter(selecter)
			, simulater(simulater)
			, backpropagater(backpropagater)
		{}

		NT* searchTree(GameState* rootState)
		{
			assert(!rootState->isTerminal());

			NT* root = new NT(nullptr);

			//TODO: constrain in time instead
			for (int i = 0; i < 1000; i++)
			{
				NT* node = root;
				GameState state(*rootState);

				//selection
				while (true)
				{
					EffectNodePair<NT>& next = selecter->select(*node, state);
					if (next.node == nullptr)
					{
						//expansion
						if (next.effect != nullptr) //state->isTerrminal() returned true
						{
							state.queueEffect(0, next.effect);
							node = next.node = new NT(node);
						}
						break;
					}
					state.queueEffect(0, next.effect);
					node = next.node;
				}

				//simulation
				double score = simulater->simulate(&state);

				//backpropagation
				backpropagater->backpropagate(node, score);

				//NodeStatePair<NodeUCT>* node = rootChild;
				//traverse(rootChild, state);
			}

			return root;
		}

		std::vector<Effect*> search(GameState* rootState) override
		{
			NT* const root = searchTree(rootState);
			NT* node = root;
			GameState state(*rootState);
			
			std::vector<Effect*> bestActions;

			//TODO: select best, not regular selection
			while (state.getFrame() == 0)
			{
				EffectNodePair<NT>& next = selecter->select(*node, state);
				if (next.node == nullptr)
					break;
				if (next.effect->isPlayerAction(&state))
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

	private:
		double traverse(EffectNodePair<NT> node, GameState& state) const
		{
			double score;

			if (node.node->visits == 0)
				score = playout(state);
			else if (!node.node->isTerminal())
				score = traverse(selectChild(node.node));
			else //already visited terminal state
				score = node.node->totalReward/node.node->visits;
			
			node.node->visits++;
			node.node->totalReward += score;
			if (node.effect->isPlayerAction(&state))
			{
				return -score;
			}
			else
			{
				return score;
			}

		}
	};
}}
