#pragma once
#include "common/common.hpp"

namespace Bot
{
	class Effect;
	class GameState;

	struct ActionLister
	{
		virtual ~ActionLister() {}
		virtual vector<shared_ptr<Effect>> operator()(const GameState& gamestate) const = 0;
	};
}
