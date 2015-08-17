#pragma once

namespace Bot { namespace Search
{
	class GameState;

	template<class NT>
	struct Selecter
	{
		virtual ~Selecter() {}
		virtual NT* select(NT* node, const GameState& state) const = 0;
	};
}}
