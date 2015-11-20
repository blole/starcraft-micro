#pragma once
#include "common/common.hpp"
#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Effects
{
	struct MoveData : OneUnitEffectData
	{
		BWAPI::Position offset;
		MoveData(const Unit& unit, const BWAPI::Position& offset)
			: OneUnitEffectData(unit)
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
		Move(const Unit& unit, const BWAPI::Position& offset)
			: Move(Data(unit, offset))
		{}
		Move(const Unit& unit, float direction)
			: Move(unit, BWAPI::Position(
				(int)(std::cos(direction) * unit.bwapiUnit->getType().topSpeed() * 10),
				(int)(std::sin(direction) * unit.bwapiUnit->getType().topSpeed() * 10)))
		{}

		virtual void applyTo(GameState& state) const override
		{
			Unit& unit = state.get(bwapiUnit());
			if (!unit.isAlive() || unit.moveCooldown || offset() == BWAPI::Position(0,0)) //can't start/continue or done
				unit.isMoving = false;
			else if (!unit.isMoving) //starting
			{
				unit.isMoving = true;
				state.queueEffect(1, std::make_shared<Move<Data>>(data));
			}
			else //continuing
			{
				double topSpeed = unit.bwapiUnit->getType().topSpeed();
				double distanceLeft = offset().getLength();
				double stepRatio = std::min(topSpeed / distanceLeft, 1.0);

				BWAPI::Position step(int(offset().x*stepRatio), int(offset().y*stepRatio));
				unit.pos += step;
				unit.isMoving = true;
				Data nextData(data);
				nextData.offset -= step;
				state.queueEffect(1, std::make_shared<Move<Data>>(nextData));
			}
		}

		virtual void applyPredicted(GameState& state) const override {}

		virtual void executeOrder(GameState& state) const override
		{
			bwapiUnit()->move(bwapiUnit()->getPosition() + offset());

			Broodwar->drawLineMap(bwapiUnit()->getPosition(), bwapiUnit()->getPosition() + offset(), BWAPI::Colors::Grey);
		}
	};
}}
