#pragma once
#include "common/general.hpp"
#include "common/punit.hpp"

namespace Bot
{
	template <class SquadType>
	class GeneralAllUnitsSingleSquad : public General
	{
		SquadType squad;
	public:
		GeneralAllUnitsSingleSquad(const SquadType& squad = SquadType())
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

				PUnit* pUnit = PUnit::get(u);

				if (pUnit->squad == nullptr)
					squad.addUnit(pUnit);
			}

			squad.onFrame();

			Broodwar->drawTextScreen(1, 0, "%d units in squad", squad.units.size());
		}
	};
}
