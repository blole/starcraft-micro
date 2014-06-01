#pragma once
#include <list>
#include "search/gamestate.hpp"
#include "search/actionlisters/actionlister.hpp"

namespace Bot { namespace Search
{
	class Searcher
	{
	public:
		virtual std::list<Action*> search(GameState* gamestate, ActionLister* possibleActions) const = 0;
	};
}}
