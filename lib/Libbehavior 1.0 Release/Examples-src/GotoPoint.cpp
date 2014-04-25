#include "GotoPoint.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <iostream>

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

//goStraight will never return SUCCESS!!! Remember to provide decorators or conditions so it will fail.

void GotoPoint::init(void* agent)
{
};

GotoPoint::GotoPoint(point _target, int _speed)
{
	speed = _speed;
	target = _target;
}
BEHAVIOR_STATUS GotoPoint::execute(void* agent)
{
	cout << "going to point " << target.toString() << endl;
	GameObject* ship = (GameObject*) agent;
	int timeElapsed = ws->timeElapsed;

	point vectorToTarget = target - ship->location;
	if (vectorToTarget.vectorSize() < 10)
		return BT_SUCCESS;
	vectorToTarget.normalize();
	vectorToTarget *= (float)(timeElapsed * speed /1000.0f);

	point finalPosition = point(vectorToTarget.x + ship->getXPosition(),vectorToTarget.y + ship->getYPosition());
	ws->moveObject(ship,finalPosition);
	return BT_RUNNING;

}