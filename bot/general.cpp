#include "bot/general.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

General::General()
{
}

void General::onStart()
{
	
}

void General::onFrame()
{
	Unitset myUnits = Broodwar->self()->getUnits();
	
	for (Unitset::iterator u = myUnits.begin(); u != myUnits.end(); ++u)
	{
		if (!u->exists()		|| u->isLockedDown()	||
			u->isMaelstrommed()	|| u->isStasised()		||
			u->isLoaded()		|| !u->isPowered()		||
			u->isStuck()		|| !u->isCompleted()	||
			u->isConstructing())
			continue;

		PUnit* pUnit = PUnit::get(*u);
		pUnit->brain->execute(pUnit);
	}
}
