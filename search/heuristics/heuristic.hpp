#pragma once

namespace Bot { namespace Search
{
	class GameState;
	
	struct Heuristic
	{
		virtual ~Heuristic() {}
		virtual double value(const GameState* state) const = 0;
	};
}}
