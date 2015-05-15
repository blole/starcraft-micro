#include "behaviors/attackclosest.hpp"
#include "common/gameunit.hpp"
#include "common/punit.hpp"

using namespace BWAPI;
using namespace Bot;
using namespace Bot::Behaviors;
using namespace BehaviorTree;

void AttackClosest::init(void* agent)
{
	first = true;
	hasStartAttack = false;
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;

	if(hasStartAttack)
	{
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		if (!pUnit->unit->isAttackFrame()) //if (pUnit->target.getUnit()->exists())
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
	else
	{
		if(pUnit->unit->getGroundWeaponCooldown() != 0)
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			PositionOrUnit target = pUnit->getClosestEnemy();
			pUnit->attackTarget(target,false);
		}
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		return BT_RUNNING;
	}
}
