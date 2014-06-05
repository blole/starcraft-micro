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
				state->addEffect(1, this);

				if (frameOffset == 0)
					unit->isMoving = true;
				else
					unit->pos += moveOffset;
			}
		}

		virtual void executeOrder(GameState* state)
		{
			BWAPI::Unit unit = state->getBwapiUnit(unitID);

			unit->move(unit->getPosition() + moveOffset * 10);
			
			BWAPI::Broodwar->drawLineMap(unit->getPosition(), unit->getPosition() + moveOffset*10, BWAPI::Colors::Grey);
		}

		virtual bool isPlayerAction(const GameState* state) const
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}

		virtual Move* clone() const
		{
			return new Move(*this);
		}
	};
}}
