#include "behaviors/attackclosest.hpp"
#include "common/unit.hpp"

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
	Unit* unit = (Unit*)agent;

	if(hasStartAttack)
	{
		Broodwar->drawLineMap(unit->getPosition(),unit->target.getUnit()->getPosition(),Color(255,0,0));
		if (!unit->bwapiUnit->isAttackFrame()) //if (pUnit->target.getUnit()->exists())
			return BT_SUCCESS;
		else
			return BT_RUNNING;
	}
	else
	{
		if(unit->bwapiUnit->getGroundWeaponCooldown() != 0)
		{
			hasStartAttack = true;
		}

		if(!hasStartAttack)
		{
			PositionOrUnit target = unit->bwapiUnit->getClosestUnit(Filter::IsEnemy);
			unit->attackTarget(target,false);
		}
		Broodwar->drawLineMap(unit->getPosition(),unit->target.getUnit()->getPosition(),Color(255,0,0));
		return BT_RUNNING;
	}
}
