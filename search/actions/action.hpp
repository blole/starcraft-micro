#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	class Action
	{
	protected:
		const int frameStarted;

		Action(const GameState* state)
			: frameStarted(state->getFrame())
		{}

	public:
		bool applyTo(GameState* state) { return applyTo(state, state->getFrame()-frameStarted); }
		virtual bool applyTo(GameState* state, int frameOffset) = 0;
		virtual void executeOrder(GameState* state) = 0;
	};





	class SingleUnitAction : public Action
	{
	protected:
		const id_t unitID;

		SingleUnitAction(const GameState* state, const Unit* unit)
			: Action(state)
			, unitID(unit->id)
		{}
	};





	class TwoUnitAction : public SingleUnitAction
	{
	protected:
		const id_t targetID;

		TwoUnitAction(const GameState* state, const Unit* unit, const Unit* target)
			: SingleUnitAction(state, unit)
			, targetID(target->id)
		{}
	};
}}
