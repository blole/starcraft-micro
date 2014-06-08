#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searchers/searcher.hpp"
#include "search/units/unit.hpp"
#include "search/actions/action.hpp"

namespace Bot { namespace Search
{
	class NodeUCT;

	class NodeChild
	{
	public:
		Action* action;
		NodeUCT* node;

		NodeChild(Action* action, NodeUCT* node = nullptr)
			: action(action)
			, node(node)
		{}
	};



	class NodeUCT
	{
	public:
		NodeUCT* const parent;
		GameState* const gamestate;
		std::vector<NodeChild> children;

		int visits;
		double totalReward;
		bool fullyExpanded;

	private:
		bool terminal;
		bool checkedForTerminal;

	public:
		NodeUCT(NodeUCT* const parent, GameState* const gamestate, ActionLister* actionlister)
			: parent(parent)
			, gamestate(gamestate)
			, visits(0)
			, totalReward(0)
			, fullyExpanded(false)
			, terminal(false)
			, checkedForTerminal(false)
		{
			std::list<Action*> actions;
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
					for each (Action* action in actions)
						children.push_back(NodeChild(action));
					break;
				}
			}
		}

		~NodeUCT()
		{
			for each (NodeChild child in children)
			{
				delete child.node;
				delete child.action;
			}

			if (parent != nullptr)
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
		std::list<Action*> search(GameState* gamestate, ActionLister* actionlister)
		{
			assert(!gamestate->isTerminal());

			this->actionlister = actionlister;

			DummyPlayerAction rootAction(gamestate);
			NodeUCT root(nullptr, gamestate, actionlister);
			NodeChild rootChild(&rootAction, &root);

			//TODO: constrain in time instead
			for (int i = 0; i < 3000; i++)
			{
				traverse(rootChild);
			}
			
			NodeUCT* node = &root;
			std::list<Action*> bestActions;
			while (node->gamestate->getFrame() == 0)
			{
				NodeChild child = selectChild(node);
				bestActions.push_back(child.action->clone());
				node = child.node;
			}

			//BWAPI::Broodwar->drawTextScreen(200, 75,  "number of taken actions: %d", bestActions.size());
			//BWAPI::Broodwar->drawTextScreen(200, 100, "root average reward: %.3f", root.totalReward / root.visits);
			BWAPI::Broodwar->drawTextScreen(0, 30,  "root.visits: %d", root.visits);
			return bestActions;
		}

	private:
		double traverse(NodeChild node)
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

		NodeChild selectChild(NodeUCT* parent)
		{
			if (!parent->fullyExpanded)
			{
				//try to find unexpanded child
				for (unsigned int i=0; i<parent->children.size(); i++)
				{
					NodeChild& child = parent->children[i];
					if (child.node == nullptr)
					{
						child.node = new NodeUCT(parent, new GameState(parent->gamestate, child.action), actionlister);
						return child;
					}
				}

				parent->fullyExpanded = true;
			}
			
			return bestChild(parent);
		}
		
		NodeChild bestChild(NodeUCT* parent)
		{
			assert(parent->fullyExpanded);

			NodeChild best = parent->children[0];
			double bestUCB = best.node->UCB(parent);

			for (unsigned int i=1; i<parent->children.size(); i++)
			{
				NodeChild child = parent->children[i];
				double ucb = child.node->UCB(parent);
				if (bestUCB < ucb)
				{
					best = child;
					bestUCB = ucb;
				}
			}
			
			return best;
		}

		double playout(GameState* gamestate)
		{
			//TODO: handle if node is terminal and step through nodes
			return evaluate(gamestate);
		}

		double evaluate(GameState* gamestate)
		{
			double sum = 0;

			for each (const Unit* unit in gamestate->playerUnits())
			{
				double cd = gamestate->getBwapiUnit(unit)->getType().groundWeapon().damageCooldown();
				double dmg = gamestate->getBwapiUnit(unit)->getType().groundWeapon().damageAmount();
				sum += std::sqrt((double)unit->hp)*dmg / cd;
			}

			for each (const Unit* unit in gamestate->enemyUnits())
			{
				double cd = gamestate->getBwapiUnit(unit)->getType().groundWeapon().damageCooldown();
				double dmg = gamestate->getBwapiUnit(unit)->getType().groundWeapon().damageAmount();
				sum -= std::sqrt((double)unit->hp)*dmg / cd;
			}

			return sum;
		}
	};
}}
