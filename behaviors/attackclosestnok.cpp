#include "behaviors/attackclosestnok.hpp"
#include "common/unit.hpp"

using namespace BWAPI;
using namespace Bot;
using namespace Bot::Behaviors;
using namespace BehaviorTree;

void AttackClosestNOK::init(void* agent)
{
	first = true;
	Unit* unit = (Unit*)agent;
	//TODO: pUnit->attackClosestEnemyNonWorried();
}

BEHAVIOR_STATUS AttackClosestNOK::execute(void* agent)
{
	Unit* unit = (Unit*)agent;

	static bool hasStartAttack = false;

	if(hasStartAttack)
	{
		if(unit->hasTarget)
			Broodwar->drawLineMap(unit->getPosition(),unit->target.getUnit()->getPosition(),Color(255,0,0));
		
		if(!unit->target.getUnit()->exists()) //|| !pUnit->isAttacking())
			return BT_SUCCESS;
		
		return BT_RUNNING;
	}
	else
	{
		if(unit->isAttacking())
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			//TODO: pUnit->attackClosestEnemyNonWorried();
		}
		
		if(unit->hasTarget)
			Broodwar->drawLineMap(unit->getPosition(),unit->target.getUnit()->getPosition(),Color(255,0,0));
		
		return BT_RUNNING;
	}
}
