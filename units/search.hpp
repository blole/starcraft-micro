#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include "units/gamestate.hpp"
#include "units/possibleactions.hpp"

namespace Bot { namespace Units
{
	class SearchAlgorithm
	{
	public:
		virtual std::list<Action*> search(GameState* gamestate, PossibleActions* possibleActions) const = 0;
	};

	class SearchUCT : public SearchAlgorithm
	{
	public:
		std::list<Action*> search(GameState* gamestate, PossibleActions* possibleActions) const;
	};
}}
