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
	public:
		static const int moveQuanta = 25;
		const BWAPI::Position& moveOffset() const { return data.moveOffset; }

	public:
		Move(const MoveData& data)
			: OneUnitEffect(data)
		{}

		Move(const Unit& unit, float direction)
			: Move(MoveData(unit.id, BWAPI::Position(
				(int)(std::cos(direction) * unit.bwapiUnit->getType().topSpeed()),
				(int)(std::sin(direction) * unit.bwapiUnit->getType().topSpeed()))))
		{}

		virtual void applyTo(GameState& state) const override
		{
			auto& unit = state.units[unitID()];
			if (!unit->isAlive() || unit->isAttackFrame)
				unit->isMoving = false;
			else
			{
				state.queueEffect(1, std::make_shared<Move>(data));

				if (unit->isMoving)
					unit->pos += moveOffset();
				unit->isMoving = true;
			}
		}

		virtual void executeOrder(GameState& state) const override
		{
			BWAPI::Unit unit = state.units[unitID()]->bwapiUnit;

			unit->move(unit->getPosition() + moveOffset() * 10);
			
			BWAPI::Broodwar->drawLineMap(unit->getPosition(), unit->getPosition() + moveOffset()*10, BWAPI::Colors::Grey);
		}
	};
}}
