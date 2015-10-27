#pragma once
#include "common/actionlisters/actionlister.hpp"
#include "common/effects/effect.hpp"
#include "common/effects/compositeeffect.hpp"
#include "common/units/unit.hpp"

namespace Bot { namespace ActionListers
{
	template <int combinations>
	struct BranchOnRandomCombinationOfAllUnitsActions : ActionLister
	{
		virtual vector<shared_ptr<Effect>> operator()(const GameState& gamestate) const override
		{
			// Create matrix of all possible actions
			vector<vector<shared_ptr<Effect>>> matrixOfActions;

			for (auto& unit : gamestate.units)
			{
				if (unit->isAlive())
				{
					vector<shared_ptr<Effect>> unitActions = unit->possibleActions(gamestate);
					if (!unitActions.empty())
						matrixOfActions.push_back(unitActions);
				}
			}

			if (matrixOfActions.empty())
				return { make_shared<AdvanceFrameEffect>() };
			else
			{
				vector<shared_ptr<Effect>> compositeActions;
				
				for (int i = 0; i < combinations; i++)
				{
					vector<shared_ptr<Effect>> actions;
					for (auto& unit_possibleActions : matrixOfActions)
						actions.push_back(unit_possibleActions[rand() % unit_possibleActions.size()]);
					compositeActions.push_back(make_shared<CompositeEffect>(actions));
				}

				return compositeActions;
			}
		}
	};
}}
