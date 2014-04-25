#include "DestroyAllNearbyProjectiles.h"
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

void DestroyAllNearbyProjectiles::init(void* agent)
{
};

DestroyAllNearbyProjectiles::DestroyAllNearbyProjectiles()
{
}
BEHAVIOR_STATUS DestroyAllNearbyProjectiles::execute(void* agent)
{
	cout << "destroying nearby projectiles" << endl;

	const GameObjectList* objects = ws->getAllGameObjects();
	ConstGameObjectIter iter;
	for (iter = objects->begin(); iter!=objects->end(); iter++)
	{
		if ((*iter)->faction != PROJECTILE_FACTION)
			continue;

		float distance = ws->distanceBetween((GameObject*) agent,*iter);
		if (distance < 200)
			ws->deleteObject(*iter);
	}
	return BT_SUCCESS;
}