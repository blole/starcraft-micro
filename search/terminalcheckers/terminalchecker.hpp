#pragma once

namespace Bot { namespace Search
{
	class GameState;
	
	struct TerminalChecker
	{
		virtual ~TerminalChecker() {}
		virtual bool operator()(const GameState& state) const = 0;
	};
}}
