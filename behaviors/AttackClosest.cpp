#include "behaviors/AttackClosest.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

AttackClosest::AttackClosest()
{
}

void AttackClosest::init(void* agent)
{
	first = true;
}

BEHAVIOR_STATUS AttackClosest::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	BWAPI::Unit unit = pUnit->unit;

	if (first)
	{
		first = false;

		Unit target = unit->getClosestUnit(Filter::IsEnemy);
		if (target == nullptr)
			return BT_SUCCESS; //no known enemies
		else if (unit->getGroundWeaponCooldown() == 0)
			unit->attack(target);
		else
			return BT_FAILURE; //already busy attacking
	}
	
	if (unit->getTarget() != nullptr)
		Broodwar->drawLineMap(unit->getPosition(), unit->getTarget()->getPosition(), Color(255, 0, 0));
	
	if (unit->getGroundWeaponCooldown() == 0)
		return BT_SUCCESS;
	else
		return BT_RUNNING;
}
