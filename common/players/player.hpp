#pragma once
#include "common/common.hpp"

namespace Bot
{
	class Effect;
	class GameState;

	struct Player
	{
		virtual ~Player() {}
		virtual vector<shared_ptr<Effect>> operator()(GameState& gamestate) = 0;
	};
}
