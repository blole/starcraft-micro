#include "turnTowardsTarget.h"
#include "point.h"
#include "globals.h"
#include "util.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;

void TurnTowardsTarget::init(void* agent)
{
};

TurnTowardsTarget::TurnTowardsTarget(float _speed)
{
	speed = _speed;
}
BEHAVIOR_STATUS TurnTowardsTarget::execute(void* agent)
{
	GameObject* object = (GameObject*) agent;
	GameObject* target = ws->getPlayer();
	
	CL_Angle angleBetween = ws->angleBetween(object,target);

	//cout << "angleBetween" << angleBetween.to_degrees() << endl;

	if ( angleBetween.to_degrees() < -(speed+1))
	{
		ws->rotateObject(object,-speed); //clockwise
		return BT_RUNNING;
	}
	else if (angleBetween.to_degrees() > speed+1)
	{
		ws->rotateObject(object,speed);
		return BT_RUNNING;
	}

	else return BT_RUNNING;
	
}