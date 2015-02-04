#include "behaviors/attackclosestnok.hpp"
#include "common/gameunit.hpp"
#include "common/punit.hpp"

using namespace BWAPI;

void AttackClosestNOK::init(void* agent)
{
	first = true;
	PUnit* pUnit = (PUnit*)agent;
	pUnit->attackClosestEnemyNonWorried();
}

BEHAVIOR_STATUS AttackClosestNOK::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	BWAPI::Unit unit = pUnit->unit;

	static bool hasStartAttack = false;

	if(hasStartAttack)
	{
		if(pUnit->hasTarget)
			Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		
		if(!pUnit->target.getUnit()->exists()) //|| !pUnit->isAttacking())
			return BT_SUCCESS;
		
		return BT_RUNNING;
	}
	else
	{
		if(pUnit->isAttacking())
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			pUnit->attackClosestEnemyNonWorried();
		}
		
		if(pUnit->hasTarget)
			Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		
		return BT_RUNNING;
	}
}
