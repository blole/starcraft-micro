#include "behaviors/moverelative.hpp"

using namespace BWAPI;

MoveRelative::MoveRelative(int x, int y)
	: offset(x,y)
{
}

BEHAVIOR_STATUS MoveRelative::firstExecute(PUnit* unit)
{
	origin = unit->getPosition();

	if (unit->getPosition() == origin + offset)
		return BT_SUCCESS;
	else
		unit->unit->move(origin + offset);

	return subsequentExecute(unit);
}

BEHAVIOR_STATUS MoveRelative::subsequentExecute(PUnit* unit)
{
	if (unit->getPosition() == origin + offset)
		return BT_SUCCESS;
	else if (unit->unit->getOrder() != Orders::Move || unit->unit->getOrderTargetPosition() != origin + offset)
		return BT_FAILURE;
	else
		return BT_RUNNING;
}
