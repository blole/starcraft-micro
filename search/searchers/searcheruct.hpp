#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searchers/searcher.hpp"
#include "search/units/unit.hpp"
#include "search/actions/effect.hpp"
#include <boost/ptr_container/clone_allocator.hpp>

namespace Bot { namespace Search { namespace UCT
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

	class NodeUCT : public NodeMCTS<NodeUCT>
	{
	public:
		GameState* const gamestate;
		bool fullyExpanded;

	private:
		bool terminal;
		bool checkedForTerminal;

	public:
		NodeUCT(NodeUCT* const parent, GameState* const gamestate, ActionLister* actionlister)
			: NodeMCTS(parent)
			, gamestate(gamestate)
			, fullyExpanded(false)
			, terminal(false)
			, checkedForTerminal(false)
		{
			std::vector<Effect*> actions;
			while (!gamestate->isTerminal())
			{
				//advance while we have not reached a terminal state and
				//there are no possible actions

				actions = actionlister->actions(gamestate);

				if (actions.size() == 0)
					gamestate->advanceFrames(1);
				else
				{
					children.reserve(actions.size());
					for (Effect* action : actions)
						children.emplace_back(action);
					break;
				}
			}
		}

		~NodeUCT()
		{
			if (!isRoot())
				delete gamestate;
		}

	public:
		bool isTerminal()
		{
			if (!checkedForTerminal)
			{
				checkedForTerminal = true;
				terminal = gamestate->isTerminal();
			}
			return terminal;
		}

		bool isRoot() const
		{
			return parent == nullptr;
		}

		double UCB(NodeUCT* parent)
		{
			return totalReward/visits + std::sqrt(std::log((double)parent->visits)/visits);
		}
	};



	class SearcherUCT : public Searcher
	{
	private:
		ActionLister* actionlister;

	public:
		NodeUCT* searchTree(GameState* state, ActionLister* actionlister)
		{
			assert(!state->isTerminal());

			this->actionlister = actionlister;

			NoEffect noEffect;
			NodeUCT* root = new NodeUCT(nullptr, state, actionlister);
			NodeUCT::Child rootChild(&noEffect, root);

			//TODO: constrain in time instead
			for (int i = 0; i < 1000; i++)
				traverse(rootChild);

			return root;
		}

		std::vector<Effect*> search(GameState* state, ActionLister* actionlister) override
		{
			NodeUCT* const root = searchTree(state, actionlister);
			NodeUCT* node = root;

			std::vector<Effect*> bestActions;
			while (node->gamestate->getFrame() == 0)
			{
				NodeUCT::Child& child = selectChild(node);
				bestActions.push_back(child.effect);
				child.effect = nullptr;
				node = child.node;
			}

			BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root->totalReward / root->visits);
			BWAPI::Broodwar->drawTextScreen(0, 30, "root.visits: %d", root->visits);
			return bestActions;
		}

	private:
		double traverse(NodeUCT::Child node) const
		{
			double score;

			if (node.node->visits == 0)
				score = playout(node.node->gamestate);
			else if (!node.node->isTerminal())
				score = traverse(selectChild(node.node));
			else //already visited terminal state
				score = node.node->totalReward/node.node->visits;
			
			node.node->visits++;
			node.node->totalReward += score;
			if (node.effect->isPlayerAction(node.node->gamestate))
			{
				return -score;
			}
			else
			{
				return score;
			}

		}

		NodeUCT::Child& selectChild(NodeUCT* parent) const
		{
			if (!parent->fullyExpanded)
			{
				//try to find unexpanded child
				for (unsigned int i=0; i<parent->children.size(); i++)
				{
					NodeUCT::Child& child = parent->children[i];
					if (child.node == nullptr)
					{
						child.node = new NodeUCT(parent, new GameState(*parent->gamestate), actionlister);
						return child;
					}
				}

				parent->fullyExpanded = true;
			}
			
			return bestChild(parent);
		}
		
		NodeUCT::Child& bestChild(NodeUCT* parent) const
		{
			assert(parent->fullyExpanded);

			NodeUCT::Child& best = parent->children[0];
			double bestUCB = best.node->UCB(parent);

			for (unsigned int i=1; i<parent->children.size(); i++)
			{
				NodeUCT::Child& child = parent->children[i];
				double ucb = child.node->UCB(parent);
				if (bestUCB < ucb)
				{
					bestUCB = ucb;
					best = child;
				}
			}
			
			return best;
		}

		double playout(GameState* gamestate) const
		{
			//TODO: handle if node is terminal and step through nodes
			return evaluate(gamestate);
		}

		double evaluate(GameState* gamestate) const
		{
			double sum = 0;

			for (const Unit* unit : gamestate->playerUnits())
			{
				double cd = unit->getBwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->getBwapiUnit()->getType().groundWeapon().damageAmount();
				sum += std::sqrt((double)unit->hp)*dmg / cd;
			}

			for (const Unit* unit : gamestate->enemyUnits())
			{
				double cd = unit->getBwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->getBwapiUnit()->getType().groundWeapon().damageAmount();
				sum -= std::sqrt((double)unit->hp)*dmg / cd;
			}

			return sum;
		}
	};
}}}
