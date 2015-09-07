#pragma once
#include "common/common.hpp"

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	struct ActionLister
	{
		virtual ~ActionLister() {}
		virtual vector<shared_ptr<Effect>> operator()(const GameState& gamestate) const = 0;
	};
}}
