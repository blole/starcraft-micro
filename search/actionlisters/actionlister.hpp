#pragma once
#include <vector>

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	struct ActionLister
	{
		virtual ~ActionLister() {}
		virtual vector<shared_ptr<Effect>> actions(const GameState& gamestate) const = 0;
	};
}}
