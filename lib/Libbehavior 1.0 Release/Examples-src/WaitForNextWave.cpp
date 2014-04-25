#include "WaitForNextWave.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"
#include <iostream>
using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;
using namespace std;

void WaitForNextWave::init(void* agent)
{
};

WaitForNextWave::WaitForNextWave()
{
}
BEHAVIOR_STATUS WaitForNextWave::execute(void* agent)
{
	if (ws->getAllGameObjects()->size() == 2) //the player and the spawner
		return BT_SUCCESS;
	else
		return BT_RUNNING;
}