#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include "search/searchers/searcher.hpp"

namespace Bot { namespace Search
{
	class SearcherUCT : public Searcher
	{
	public:
		std::list<Action*> search(GameState* gamestate, ActionLister* possibleActions) const
		{
			std::vector<Action*> bestActions;
			//abort();
			std::list<Action*> a = possibleActions->actions(gamestate);
			BWAPI::Broodwar->drawTextScreen(200, 100,  "possible actions: %d", a.size());
			return a;
			//return bestActions;
		}
	};
}}
