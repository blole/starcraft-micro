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
	class Node;

	class ActionNodePair
	{
	public:
		Effect* action;
		Node* node;

		ActionNodePair(Effect* action, Node* node = nullptr)
			: action(action)
			, node(node)
		{}
		~ActionNodePair()
		{
			delete action;
			delete node;
		}
	};



	class Node
	{
	public:
		Node* const parent;
		GameState* const gamestate;
		std::vector<ActionNodePair> children;

		int visits;
		double totalReward;
		bool fullyExpanded;

	private:
		bool terminal;
		bool checkedForTerminal;

	public:
		Node(Node* const parent, GameState* const gamestate, ActionLister* actionlister)
			: parent(parent)
			, gamestate(gamestate)
			, visits(0)
			, totalReward(0)
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

		~Node()
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

		double UCB(Node* parent)
		{
			return totalReward/visits + std::sqrt(std::log((double)parent->visits)/visits);
		}
	};



	class SearcherUCT : public Searcher
	{
	private:
		ActionLister* actionlister;

	public:
		std::vector<Effect*> search(GameState* gamestate, ActionLister* actionlister)
		{
			assert(!gamestate->isTerminal());

			this->actionlister = actionlister;

			NoEffect noEffect;
			Node root(nullptr, gamestate, actionlister);
			ActionNodePair rootChild(&noEffect, &root);

			//TODO: constrain in time instead
			for (int i = 0; i < 1000; i++)
				traverse(rootChild);
			
			//get root actions
			Node* node = &root;
			std::vector<Effect*> bestActions;
			while (node->gamestate->getFrame() == 0)
			{
				ActionNodePair& child = selectChild(node);
				child.action = nullptr;
				bestActions.push_back(child.action);
				node = child.node;
			}

			//BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			//BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root.totalReward / root.visits);
			BWAPI::Broodwar->drawTextScreen(0, 30,  "root.visits: %d", root.visits);
			return bestActions;
		}

	private:
		double traverse(ActionNodePair node) const
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
			if (node.action->isPlayerAction(node.node->gamestate))
			{
				return -score;
			}
			else
			{
				return score;
			}

		}

		ActionNodePair& selectChild(Node* parent) const
		{
			if (!parent->fullyExpanded)
			{
				//try to find unexpanded child
				for (unsigned int i=0; i<parent->children.size(); i++)
				{
					ActionNodePair& child = parent->children[i];
					if (child.node == nullptr)
					{
						child.node = new Node(parent, new GameState(*parent->gamestate), actionlister);
						return child;
					}
				}

				parent->fullyExpanded = true;
			}
			
			return bestChild(parent);
		}
		
		ActionNodePair& bestChild(Node* parent) const
		{
			assert(parent->fullyExpanded);

			ActionNodePair& best = parent->children[0];
			double bestUCB = best.node->UCB(parent);

			for (unsigned int i=1; i<parent->children.size(); i++)
			{
				ActionNodePair& child = parent->children[i];
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
