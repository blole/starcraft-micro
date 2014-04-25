#include "WaitForSignal.h"
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

void WaitForSignal::init(void* agent)
{
};

WaitForSignal::WaitForSignal(bool& _signal):signal(_signal)
{
}
BEHAVIOR_STATUS WaitForSignal::execute(void* agent)
{
	if (signal)
	{
		cout << "got signal!" << endl;
		return BT_SUCCESS;
	}
	else
	{
		return BT_RUNNING;
	}
}