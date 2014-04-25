#include "Cooldown.h"
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

void Cooldown::init(void* agent)
{
	current = -1;
};

Cooldown::Cooldown(int timeout)
{
	limit = timeout;
	current = -1;
}
BEHAVIOR_STATUS Cooldown::execute(void* agent)
{
	current += ws->timeElapsed;
	if (current >= limit)
	{
		current = -1;
		return BT_SUCCESS;
	}
	else
		return BT_RUNNING;
}