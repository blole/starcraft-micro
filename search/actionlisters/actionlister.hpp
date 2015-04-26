#pragma once
#include <BWAPI.h>
#include <vector>

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	class ActionLister
	{
	public:
		virtual std::vector<Effect*> actions(const GameState* gamestate) const = 0;
	};
}}
