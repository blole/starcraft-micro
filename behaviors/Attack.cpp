#include "behaviors/Attack.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

Attack::Attack()
{
}

void Attack::init(void* agent)
{
	first = true;
	hasStartAttack = false;
	hasBeenOrderToAttack = false;
}

BEHAVIOR_STATUS Attack::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;

	if(hasStartAttack)
	{
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		if(!pUnit->unit->isAttackFrame())
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
			hasBeenOrderToAttack = true;
		}
		Broodwar->drawLineMap(pUnit->getPosition(),pUnit->target.getUnit()->getPosition(),Color(255,0,0));
		return BT_RUNNING;
	}
}
