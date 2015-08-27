#pragma once
#include "search/actionlisters/actionlister.hpp"
#include "search/effects/effect.hpp"
#include "search/effects/compositeeffect.hpp"
#include "search/units/unit.hpp"

namespace Bot { namespace Search { namespace ActionListers
{
	class BranchOnPlayer : public ActionLister
	{
	public:
		virtual std::vector<Effect*> actions(const GameState& gamestate) const override
		{

			std::vector<Effect*> setOfActions; // Output of the function

			// Create matrix of all possible actions
			std::list<std::vector<Effect*>> matrixOfActions;

			for (auto& unit : gamestate.units)
			{
				if (unit->isAlive() && unit->isPlayerUnit() == current_player)
				{
					std::vector<Effect*> list = unit->possibleActions(gamestate);
					std::vector<Effect*> actions(list);
					if(!actions.empty())
						matrixOfActions.push_back(actions);
				}
			}

			// Create a list of Composite action from random combination

			for(int nbPerm = 0; nbPerm < nbPermMax ; nbPerm++)
			{
				std::vector<Effect*> actions;
				for(auto i=matrixOfActions.begin(); i!=matrixOfActions.end();i++)
				{
					int k = rand() % (*i).size();
					actions.push_back((*i).at(k));
				}
				if(!actions.empty())
					setOfActions.push_back(new CompositeEffect(actions));
			}
			
			//abort();
			return setOfActions;
		}

		void switchPlayer(bool player){
			current_player = player;
		}

	private:
		bool current_player;
		const static int nbPermMax = 12;
	};
}}}
