#include "SelfDestruct.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

void SelfDestruct::init(void* agent)
{
};

SelfDestruct::SelfDestruct()
{
}
BEHAVIOR_STATUS SelfDestruct::execute(void* agent)
{
	Ship* ship = (Ship*) agent;
	ws->deleteObject(ship);
	return BT_SUCCESS;
}