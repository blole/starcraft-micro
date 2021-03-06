#pragma once
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/terminalcheckers/terminalchecker.hpp"

namespace Bot { namespace TerminalCheckers
{
	struct Standard : TerminalChecker
	{
		virtual bool operator()(const GameState& state) const override
		{
			static auto isAlive = [](const Unit* u) {return u->isAlive();};
			return
				std::none_of(state.playerUnits.begin(), state.playerUnits.end(), isAlive) ||
				std::none_of(state.enemyUnits.begin(),  state.enemyUnits.end(),  isAlive);
		}
	};
}}
