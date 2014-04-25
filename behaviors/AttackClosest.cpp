#include "behaviors/AttackClosest.hpp"
#include "AttackClosest/GameUnit.hpp"
#include "AttackClosest/PUnit.hpp"

using namespace BWAPI;

AttackClosest::AttackClosest()
{
}

void AttackClosest::init(void* agent)
{
	PUnit *pUnit = (PUnit*)agent;
	target = pUnit->getClosestEnnemy();
	pUnit->attackTarget(target);
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
	PUnit *pUnit = (PUnit*)agent;

	if(!target->exists())
	{ 
		Broodwar << "Sucess" << std::endl;
		return BT_SUCCESS;
	}
	else
	{
		Unit tmpTarget = pUnit->getClosestEnnemy();
		if(target!=tmpTarget)
		{
			target = tmpTarget;
			pUnit->attackTarget(target);
		}
		return BT_RUNNING;
	}
}
