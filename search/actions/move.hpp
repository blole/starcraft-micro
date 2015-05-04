#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	struct MoveData : public OneUnitEffectData
	{
		BWAPI::Position moveOffset;
		MoveData(const id_t& unitID, const BWAPI::Position& moveOffset)
			: OneUnitEffectData(unitID)
			, moveOffset(moveOffset)
		{}
	};

	class Move
		: public OneUnitEffect<MoveData>
	{
		static const int moveQuanta = 25;
		const BWAPI::Position& moveOffset;
	public:
		Move(const Unit* unit, float direction)
			: Move(MoveData(unitID, BWAPI::Position(
				(int)(std::cos(direction) * unit->getBwapiUnit()->getType().topSpeed()),
				(int)(std::sin(direction) * unit->getBwapiUnit()->getType().topSpeed()))))
		{}

		Move(const MoveData& data)
			: OneUnitEffect(data)
			, moveOffset(data.moveOffset)
		{}

		virtual void applyTo(GameState* state) const override
		{
			Unit* unit = state->getUnitModifiable(unitID);
			if (!unit->isAlive() || unit->isAttackFrame)
			{
				unit->isMoving = false;
			}
			else
			{
				state->queueEffect(1, new Move(data));

				if (unit->isMoving)
					unit->pos += moveOffset;
				unit->isMoving = true;
			}
		}

		virtual void executeOrder(GameState* state) const override
		{
			BWAPI::Unit unit = state->getBwapiUnit(unitID);

			unit->move(unit->getPosition() + moveOffset * 10);
			
			BWAPI::Broodwar->drawLineMap(unit->getPosition(), unit->getPosition() + moveOffset*10, BWAPI::Colors::Grey);
		}

		virtual bool isPlayerAction(const GameState* state) const override
		{
			return state->getUnit(unitID)->isPlayerUnit();
		}
	};
}}
