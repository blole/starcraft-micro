#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <cassert>
#include "search/searchers/searcher.hpp"
#include "search/units/unit.hpp"
#include "search/actionlisters/branchonplayer.hpp"

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

		static int countNode;

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
			NodeABCD::countNode++;

			std::list<Action*> actions;
			actions = actionlister->actions(gamestate);

			children.reserve(actions.size());
			for each (Action* action in actions)
				children.push_back(NodeChild(action));
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

	int NodeABCD::countNode;

	class SearcherABCD : public Searcher
	{
	private:
		ActionLister* actionlister;
		int nbcall;
		int nbPrune;
		int nbFrameAdvanced;

	public:
		std::list<Action*> search(GameState* gamestate, ActionLister* actionlister)
		{
			this->actionlister = actionlister;
			((BranchOnPlayer*)this->actionlister)->switchPlayer(true); //
			NodeABCD* root = new NodeABCD(nullptr, gamestate, actionlister);

			std::vector<double> valueChildren;
			nbcall = 0;
			nbPrune = 0;
			nbFrameAdvanced = 0;
			NodeABCD::countNode = 0;
			if(root->children.empty())
				return std::list<Action*>();
			else
			{
				for each(auto child in root->children)
				{
					auto childGenerated = new NodeABCD(root, new GameState(root->gamestate, child.action), actionlister);
					valueChildren.push_back(alphabeta(childGenerated,6,-10000,10000,false,this->actionlister));
				}
			}

			double bestvalue = -100000;
			int bestindex = -1;
			for(unsigned int i=0; i < valueChildren.size(); i++)
			{
				if(bestvalue < valueChildren[i])
				{
					bestvalue = valueChildren[i];
					bestindex = i;
				}
			}

			std::list<Action*> result;
			result.push_back(root->children.at(bestindex).action);
			BWAPI::Broodwar << "Alpha-beta been called: " << nbcall << " and pruned " << nbPrune << " states." << std::endl;
			BWAPI::Broodwar << NodeABCD::countNode << " node created." << std::endl;
			BWAPI::Broodwar << nbFrameAdvanced << " advances in frame." << std::endl;
			
			return result;
		}

		double alphabeta(NodeABCD* node, int depth, double alpha, double beta, bool player, ActionLister* actionlister)
		{
			nbcall++;
			if(depth == 0 || node->isTerminal())
				return evaluate(node->gamestate);

			if(!node->children.empty()) // "normal" alpha-beta !
			{
				for each(auto child in node->children)
				{
					((BranchOnPlayer*)actionlister)->switchPlayer(!player);
					auto childGenerated = new NodeABCD(node, new GameState(node->gamestate, child.action), actionlister);
					double v = alphabeta(childGenerated, depth-1,alpha,beta,!player, actionlister);
					if(player && v > alpha) 
						alpha = v;
					if(!player && v < beta)
						beta = v;
					if(alpha >= beta)
					{
						nbPrune++;
						break;
					}
				}
				return (player ? alpha : beta);
			}
			else // try to change player
			{
				((BranchOnPlayer*)actionlister)->switchPlayer(!player);
				auto childGenerated = new NodeABCD(node, node->gamestate, actionlister);
				if(!childGenerated->children.empty())
				{
					return alphabeta(childGenerated,depth,alpha,beta,!player,actionlister);
				}
				else // Advance in time (no actions available for both player)
				{
					nbFrameAdvanced++;
					node->gamestate->advanceFrames(1);
					if(node->gamestate->isTerminal() ||nbFrameAdvanced > 100)
						return evaluate(node->gamestate);
					else
					{
						return alphabeta(node,depth,alpha,beta,player,actionlister);
					}
				}
			}

		}

	private:
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
