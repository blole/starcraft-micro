#pragma once
#include "common/terminalcheckers/standard.hpp"

namespace Bot { namespace TerminalCheckers
{
	template <int max_frame>
	struct FrameLimited : Standard
	{
		virtual bool operator()(const GameState& state) const override
		{
			return state.frameOffset() > max_frame || Standard::operator()(state);
		}
	};
}}
