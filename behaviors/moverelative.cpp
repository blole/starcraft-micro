#include "behaviors/moverelative.hpp"

using namespace Bot;
using namespace Bot::Behaviors;
using namespace BehaviorTree;

MoveRelative::MoveRelative(int x, int y)
	: offset(x,y)
{
}

BEHAVIOR_STATUS MoveRelative::firstExecute(Unit* unit)
{
	origin = unit->getPosition();

	if (unit->getPosition() == origin + offset)
		return BT_SUCCESS;
	else
		unit->bwapiUnit->move(origin + offset);

	return BT_RUNNING;
}

BEHAVIOR_STATUS MoveRelative::subsequentExecute(Unit* unit)
{
	if (unit->getPosition() == origin + offset)
		return BT_SUCCESS;
	else if (unit->bwapiUnit->getOrder() != BWAPI::Orders::Move || unit->bwapiUnit->getOrderTargetPosition() != origin + offset)
		return BT_FAILURE;
	else
		return BT_RUNNING;
}
