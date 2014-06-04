#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searchers/searcher.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search
{
	class NodeABCD;

	struct NodeChild
	{
		Action* const action;
		NodeABCD* node;

		NodeChild(Action* const action)
			: action(action)
			, node(nullptr)
		{}
	};



	class NodeABCD
	{
	public:
		NodeABCD* const parent;
		GameState* const gamestate;
		std::vector<NodeChild> children;

		int visits;
		double score;
		bool fullyExpanded;

	private:
		bool terminal;
		bool checkedForTerminal;

	public:
		NodeABCD(NodeABCD* const parent, GameState* const gamestate, ActionLister* actionlister)
			: parent(parent)
			, gamestate(gamestate)
			, visits(0)
			, score(0)
			, fullyExpanded(false)
			, terminal(false)
			, checkedForTerminal(false)
		{
			std::list<Action*> actions;
			/*while (true)
			{
				actions = actionlister->actions(gamestate);

				if (actions.size() == 0)
				{
					gamestate->advanceFrames(1);
					if (gamestate->isTerminal())
					{

						break;
					}
				}
				else
					break;
			}

			children.reserve(actions.size());
			for each (Action* action in actions)
				children.push_back(NodeChild(action));*/
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
	};



	class SearcherABCD : public Searcher
	{
	private:
		ActionLister* actionlister;

	public:
		std::list<Action*> search(GameState* gamestate, ActionLister* actionlister)
		{
			this->actionlister = actionlister;

			NodeABCD root(nullptr, gamestate, actionlister);
			//abort();
			//TODO: constrain in time instead
			for (int i = 0; i < 500; i++)
			{
				NodeABCD* leaf = treePolicy(&root);
				double score = playout(leaf->gamestate);
			}
			
			NodeABCD* node = &root;
			std::list<Action*> bestActions;
			while (node->gamestate->getFrame() == 0)
			{
				NodeChild child = bestChild(node);
				bestActions.push_back(child.action);
				node = child.node;
			}
			std::list<Action*> a = actionlister->actions(gamestate);
			BWAPI::Broodwar->drawTextScreen(200, 100,  "possible actions: %d", a.size());
			return a;
			//return bestActions;
		}

	private:
		NodeABCD* treePolicy(NodeABCD* node)
		{
			while (true)
			{
				if (node->isTerminal())
					return node;
				else if (!node->fullyExpanded)
					return expand(node);
				else
					node = bestChild(node).node;
			}
		}

		NodeABCD* expand(NodeABCD* parent)
		{
			NodeABCD* expanded = nullptr;
			for (unsigned int i=0; i<parent->children.size(); i++)
			{
				NodeChild& child = parent->children[i];
				if (child.node == nullptr) //unexpanded child found
				{
					if (expanded == nullptr) //it's the first we found
						expanded = child.node = new NodeABCD(parent, new GameState(parent->gamestate, child.action), actionlister);
					else	//we found another one, so break
						return expanded;
				}
			}

			assert(expanded != nullptr);

			//if we got here, the parent has been fully expanded
			parent->fullyExpanded = true;
			return expanded;
		}

		NodeChild bestChild(NodeABCD* parent)
		{
			assert(parent->fullyExpanded);

			NodeChild* best = &parent->children[0];

			for (unsigned int i=1; i<parent->children.size(); i++)
			{
				NodeChild* child = &parent->children[i];
				if (best->node->score < child->node->score)
					best = child;
			}

			return *best;
		}

		double traverse(NodeABCD* node)
		{
			if (node->visits == 0)
			{
				node->score = playout(node->gamestate);
			}
			else if (!node->isTerminal())
			{
				NodeChild child = selectChild(node);
 				traverse(child.node);
			}
			node->visits++;


			return node->score;
		}

		NodeChild selectChild(NodeABCD* parent)
		{
			if (parent->children.size() == 0)
				return nullptr;

			double bestScore = -std::numeric_limits<double>::infinity();
			for (unsigned int i = 0; i < parent->children.size(); i++)
			{
				NodeChild& child = parent->children[i];
				if (child.node == nullptr)
				{
					child.node = new NodeABCD(parent, new GameState(parent->gamestate, child.action), actionlister);
				}
			}
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
