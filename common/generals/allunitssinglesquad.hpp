#pragma once
#include "common/generals/general.hpp"
#include "common/units/unit.hpp"

namespace Bot { namespace Generals
{
	template <class SquadType>
	struct AllUnitsSingleSquad : General
	{
	private:
		SquadType squad;

	public:
		AllUnitsSingleSquad(const SquadType& squad = SquadType())
			: squad(squad)
		{}

	public:
		virtual void onFrame() override
		{
			for (BWAPI::Unit u : Broodwar->self()->getUnits())
			{
				if (!u->exists() || !u->getType().canAttack() ||
					u->getType().isWorker() || u->getType().isBuilding() ||
					!u->isCompleted())
					continue;

				Unit& unit = Unit::get(u);

				if (unit.squad == nullptr)
					squad.add(unit);
			}

			squad.onFrame();

			Broodwar->drawTextScreen(1, 0, "%d units in squad", squad.units().size());
		}
	};
}}
