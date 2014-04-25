#include "behaviors/moverelative.hpp"

using namespace BWAPI;

MoveRelative::MoveRelative(int x, int y)
	: offset(x,y)
{
}
void MoveRelative::init(void* agent)
{
	Unit unit = (Unit)agent;
	origin = unit->getPosition();
}

BEHAVIOR_STATUS MoveRelative::execute(void* agent)
{
	Unit unit = (Unit)agent;
	
	if (unit->getPosition() == origin+offset)
		return BT_SUCCESS;
	else
	{
		unit->move(origin+offset);
		return BT_RUNNING;
	}
}
