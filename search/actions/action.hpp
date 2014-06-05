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
		void applyTo(GameState* state) { applyTo(state, state->getFrame()-frameStarted); }
		virtual void applyTo(GameState* state, int frameOffset) = 0;
		virtual void executeOrder(GameState* state) = 0;
		virtual bool isPlayerAction(const GameState* state) const = 0;
	};
	
	
	
	
	
	class CompositeAction : public Action
	{
	protected:
		std::list<Action*> actions;
	public:

		CompositeAction(const GameState* state, std::list<Action*> actions)
			: Action(state)
			, actions(actions)
		{}

		virtual void applyTo(GameState* state, int frameOffset)
		{
			for each (Action* action in actions)
				action->applyTo(state, frameOffset);
		}

		virtual void executeOrder(GameState* state)
		{
			Broodwar << "Will execute : " << actions.size() << " actions." << std::endl;
			for each (Action* action in actions)
				action->executeOrder(state);
		}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return actions.front()->isPlayerAction(state);
		}
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
