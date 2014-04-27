#include "behaviors/moverelative.hpp"

using namespace BWAPI;

MoveRelative::MoveRelative(int x, int y)
	: offset(x,y)
{
}

BEHAVIOR_STATUS MoveRelative::firstExecute(PUnit* unit)
{
	origin = unit->getPosition();
	return subsequentExecute(unit);
}

BEHAVIOR_STATUS MoveRelative::subsequentExecute(PUnit* unit)
{
	if (unit->getPosition() == origin + offset)
		return BT_SUCCESS;
	else
	{
		unit->unit->move(origin + offset);
		return BT_RUNNING;
	}
}
