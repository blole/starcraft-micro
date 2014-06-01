#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include "search/gamestate.hpp"

namespace Bot { namespace Search
{
	class ActionLister
	{
	public:
		virtual std::list<Action*> actions(const GameState* gamestate) const = 0;
	};
}}
