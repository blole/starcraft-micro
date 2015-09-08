#pragma once

namespace Bot
{
	class GameState;

	template<class NT>
	struct Selecter
	{
		virtual ~Selecter() {}
		virtual NT& operator()(const GameState& state, NT& node) const = 0;
	};
}
