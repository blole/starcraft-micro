#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include "units/gamestate.hpp"

namespace Bot { namespace Units
{
	class PossibleActions
	{
	public:
		virtual std::list<Action*> actions(const GameState* gamestate) const = 0;
	};

	class BranchOnUnit : public PossibleActions
	{
	public:
		virtual std::list<Action*> actions(const GameState* gamestate) const;
	};
}}
