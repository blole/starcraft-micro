#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/actions/action.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	class Move : public SingleUnitAction
	{
		static const int moveQuanta = 25;
		const BWAPI::Position moveOffset;
	public:
		Move(const GameState* state, const Unit* unit, float direction)
			: SingleUnitAction(state, unit)
			, moveOffset(
				BWAPI::Position(
					(int)(std::cos(direction)*state->getBwapiUnit(unit)->getType().topSpeed()),
					(int)(std::sin(direction)*state->getBwapiUnit(unit)->getType().topSpeed())))
		{}

		virtual void applyTo(GameState* state, int frameOffset)
		{
			Unit* unit = state->getUnitModifiable(unitID);
			if (!unit->isAlive() || unit->isAttackFrame || frameOffset >= moveQuanta)
			{
				unit->isMoving = false;
			}
			else
			{
				state->enqueueEffect(1, this);

				if (frameOffset == 0)
					unit->isMoving = true;
				else
					unit->pos += moveOffset;
			}
		}

		virtual void executeOrder(GameState* state)
		{
			state->getBwapiUnit(unitID)->move(moveOffset * 3);
		}

		virtual bool isPlayerAction(GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}
	};
}}
