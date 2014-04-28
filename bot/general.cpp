#include "bot/general.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

General::General(std::function<Squad*()> newSquad)
	: newSquad(newSquad)
{
}

void General::onStart()
{
}

void General::onFrame()
{
	for each (BWAPI::Unit u in Broodwar->self()->getUnits())
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
	
	for each (Squad* squad in squads)
		squad->onFrame();
	
	//text overlay
	Broodwar->drawTextScreen(1, 0, "squads: %d", squads.size());
	int i = 0;
	for each (Squad* squad in squads)
	{
		Broodwar->drawTextScreen(1, (i+1)*15, "squad %d: %d units", i, squad->units.size());
		i++;
	}
}
