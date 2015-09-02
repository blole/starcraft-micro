#pragma once
#include <BWAPI.h>
#include "search/players/player.hpp"
#include "search/units/unit.hpp"
#include "search/actionlisters/branchonplayer.hpp"
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class NodeABCD;

	struct NodeChild
	{
		Effect* const action;
		NodeABCD* node;

		NodeChild(Effect* const action)
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

			std::vector<Effect*> actions;
			actions = actionlister->actions(gamestate);

			children.reserve(actions.size());
			for (Effect* action : actions)
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
		std::vector<Effect*> operator()(GameState* gamestate, ActionLister* actionlister)
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
				return std::vector<Effect*>();
			else
			{
				for (auto child : root->children)
				{
					GameState* newState = new GameState(*root->gamestate);
					newState->queueEffect(0, child.action);
					auto childGenerated = new NodeABCD(root, newState, actionlister);
					valueChildren.push_back(alphabeta(childGenerated,20,-10000,10000,false,this->actionlister));
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

			std::vector<Effect*> result;
			result.push_back(root->children.at(bestindex).action);
			BWAPI::Broodwar << "Alpha-beta been called: " << nbcall << " and pruned " << nbPrune << " states." << std::endl;
			BWAPI::Broodwar << NodeABCD::countNode << " node created." << std::endl;
			BWAPI::Broodwar << nbFrameAdvanced << " advances in frame." << std::endl;
			BWAPI::Broodwar << bestvalue << " best value." << std::endl;
			
			return result;
		}

		double alphabeta(NodeABCD* node, int depth, double alpha, double beta, bool player, ActionLister* actionlister)
		{
			nbcall++;
			if(depth == 0 || node->isTerminal())
				return evaluate(node->gamestate);

			if(!node->children.empty()) // "normal" alpha-beta !
			{
				for (auto child : node->children)
				{
					((BranchOnPlayer*)actionlister)->switchPlayer(!player);
					GameState* newState = new GameState(*node->gamestate);
					newState->queueEffect(0, child.action);
					auto childGenerated = new NodeABCD(node, newState, actionlister);
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

			for (const Unit* unit : gamestate->playerUnits())
			{
				double cd = unit->bwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->bwapiUnit()->getType().groundWeapon().damageAmount();
				sum += std::sqrt((double)unit->hp)*dmg / cd;
			}

			for (const Unit* unit : gamestate->enemyUnits())
			{
				double cd = unit->bwapiUnit()->getType().groundWeapon().damageCooldown();
				double dmg = unit->bwapiUnit()->getType().groundWeapon().damageAmount();
				sum -= std::sqrt((double)unit->hp)*dmg / cd;
			}

			return sum;
		}
	};
}}
