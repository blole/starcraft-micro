#pragma once
#include "search/terminalcheckers/standard.hpp"

namespace Bot { namespace Search { namespace TerminalCheckers
{
	struct FrameLimited : Standard
	{
		const unsigned int max_frame;

		FrameLimited(unsigned int max_frame)
			: max_frame(max_frame)
		{}

		virtual bool operator()(const GameState& state) const override
		{
			return state.frame() > max_frame || Standard::operator()(state);
		}
	};
}}}
