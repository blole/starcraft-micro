#pragma once
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	class Searcher
	{
	public:
		virtual ~Searcher() {}
		virtual vector<shared_ptr<Effect>> operator()(GameState& gamestate) = 0;
	};
}}
