#pragma once
#include "search/node.hpp"

namespace Bot { namespace Search
{
	class GameState;

	template<class NT>
	struct Selecter
	{
		virtual NT& select(NT* node, const GameState& state) const = 0;
	};
}}
