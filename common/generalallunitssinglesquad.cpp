#include "common/generalallunitssinglesquad.hpp"
#include "common/punit.hpp"

using namespace BWAPI;

void GeneralAllUnitsSingleSquad::onFrame()
{
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
		Broodwar->drawTextScreen(1, (i+1)*15, "squad %d: %d units", i, squad->units.size());
		i++;
	}
}
