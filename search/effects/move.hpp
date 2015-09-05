#pragma once
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"
#include <BWAPI.h>

namespace Bot { namespace Search
{
	struct MoveData : OneUnitEffectData
	{
		BWAPI::Position offset;
		MoveData(const id_t& unitID, const BWAPI::Position& offset)
			: OneUnitEffectData(unitID)
			, offset(offset)
		{}
	};

	template <class Data = MoveData>
	class Move : public OneUnitEffect<Data>
	{
	public:
		const BWAPI::Position& offset() const { return data.offset; }

	public:
		Move(const Data& data)
			: OneUnitEffect(data)
		{}

		Move(const Unit& unit, float direction)
			: Move(Data(unit.id, BWAPI::Position(
				(int)(std::cos(direction) * unit.bwapiUnit->getType().topSpeed() * 10),
				(int)(std::sin(direction) * unit.bwapiUnit->getType().topSpeed() * 10))))
		{}

		virtual void applyTo(GameState& state) const override
		{
			auto& unit = state.units[unitID()];
			if (!unit->isAlive() || unit->isAttackFrame || offset() == BWAPI::Position(0,0)) //can't start/continue or done
				unit->isMoving = false;
			else if (!unit->isMoving) //starting
			{
				unit->isMoving = true;
				state.queueEffect(1, std::make_shared<Move<Data>>(data));
			}
			else //continuing
			{
				double topSpeed = unit->bwapiUnit->getType().topSpeed();
				double distanceLeft = offset().getLength();
				double stepRatio = std::min(topSpeed / distanceLeft, 1.0);

				BWAPI::Position step(int(offset().x*stepRatio), int(offset().y*stepRatio));
				unit->pos += step;
				unit->isMoving = true;
				Data nextData(data);
				nextData.offset -= step;
				state.queueEffect(1, std::make_shared<Move<Data>>(nextData));
			}
		}

		virtual void executeOrder(GameState& state) const override
		{
			BWAPI::Unit unit = state.units[unitID()]->bwapiUnit;

			unit->move(unit->getPosition() + offset());
			
			Broodwar->drawLineMap(unit->getPosition(), unit->getPosition() + offset(), BWAPI::Colors::Grey);
		}
	};
}}
