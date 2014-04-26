#include "behaviors/AttackClosest.hpp"
#include "CustomUnits/GameUnit.hpp"
#include "CustomUnits/OUnit.hpp"
#include "CustomUnits/PUnit.hpp"

using namespace BWAPI;

AttackClosest::AttackClosest()
{
}

void AttackClosest::init(void* agent)
{
	PUnit *pUnit = (PUnit*)agent;
	target = pUnit->getClosestEnnemy();
	if(!(target == nullptr))
		pUnit->attackTarget(target);
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
	PUnit *pUnit = (PUnit*)agent;

	static bool hasStartAttack = false;

	if(hasStartAttack)
	{
		Broodwar->drawLineMap(pUnit->getPosition(),target->getPosition(),Color(255,0,0));
		if(!target->exists())
			return BT_SUCCESS;
	}
	else
	{
		if(pUnit->isAttacking())
		{
			hasStartAttack = true;
			Broodwar << "Started attack" << std::endl;
		}

		if(!hasStartAttack)
		{
			Unit tmpTarget = pUnit->getClosestEnnemy();
			if(tmpTarget == nullptr)
				return BT_SUCCESS;
			if(target!=tmpTarget)
			{
				target = tmpTarget;
				pUnit->attackTarget(target);
			}
		}
		Broodwar->drawLineMap(pUnit->getPosition(),target->getPosition(),Color(255,0,0));
		return BT_RUNNING;
	}
}
