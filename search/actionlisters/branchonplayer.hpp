#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/units/unit.hpp"
#include <stdlib.h>     /* srand, rand */

namespace Bot { namespace Search
{
	class BranchOnPlayer : public ActionLister
	{
	public:
		std::list<Action*> actions(const GameState* gamestate) const
		{

			std::list<Action*> setOfActions; // Output of the function

			// Create matrix of all possible actions
			std::list<std::vector<Action*>> matrixOfActions;
			const std::vector<const Unit*>& units = gamestate->getUnits();

			for each(const Unit* unit in units)
			{
				if (unit->isAlive())
				{
					std::list<Action*> list = unit->possibleActions(gamestate);
					std::vector<Action*> actions = std::vector<Action*>(list.begin(),list.end());
					if(!actions.empty() && actions.front()->isPlayerAction(gamestate) == current_player)
						matrixOfActions.push_back(actions);
				}
			}

			// Create a list of Composite action from random combination
			for(int nbPerm = 0; nbPerm < nbPermMax ; nbPerm++)
			{
				std::list<Action*> actions;
				for(auto i=matrixOfActions.begin(); i!=matrixOfActions.end();i++)
				{
					int k = rand() % (*i).size();
					actions.push_back((*i).at(k));
				}
				setOfActions.push_back(new CompositeAction(gamestate, actions));
			}
			
			//abort();
			return setOfActions;
		}

		void switchPlayer(bool player){
			current_player = player;
		}

	private:
		bool current_player;
		const static int nbPermMax = 5;
	};
}}
