#pragma once
#include <vector>

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	class Searcher
	{
	public:
		virtual std::vector<Effect*> search(GameState* gamestate) = 0;
	};
}}
