#pragma once
#include <BWAPI.h>
#include <list>
#include "common/general.hpp"
#include "common/punit.hpp"

namespace Bot
{
	class GeneralAllUnitsSingleSquad : public General
	{
	public:
		explicit GeneralAllUnitsSingleSquad(std::function<Squad*()>& newSquad)
			: General(newSquad)
		{}
		explicit GeneralAllUnitsSingleSquad(std::function<BehaviorTree::BehaviorTreeNode*()> unitBrain)
			: General(unitBrain)
		{}
	
	
	public:
		virtual void onFrame() override
		{
			using namespace BWAPI;

			for (BWAPI::Unit u : Broodwar->self()->getUnits())
			{
				if (!u->exists() || !u->getType().canAttack() ||
					u->getType().isWorker() || u->getType().isBuilding() ||
					!u->isCompleted())
					continue;

				PUnit* pUnit = PUnit::get(u);

				if (pUnit->squad == nullptr)
				{
					if (squads.empty())
						squads.push_back(newSquad());

					squads.front()->addUnit(pUnit);
				}
			}

			for (Squad* squad : squads)
				squad->onFrame();

			//text overlay
			Broodwar->drawTextScreen(1, 0, "squads: %d", squads.size());
			int i = 0;
			for (Squad* squad : squads)
			{
				Broodwar->drawTextScreen(1, (i + 1) * 15, "squad %d: %d units", i, squad->units.size());
				i++;
			}
		}
	};
}
