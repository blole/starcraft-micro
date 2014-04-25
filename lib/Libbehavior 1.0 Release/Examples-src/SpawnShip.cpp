#include "SpawnShip.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"
#include <iostream>

#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"
#include "BehaviorFactory.h"
#include "GotoPoint.h"

using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;
using namespace std;

void SpawnShip::init(void* agent)
{
};

SpawnShip::SpawnShip(int ship)
{
	shipType = ship;
}
BEHAVIOR_STATUS SpawnShip::execute(void* agent)
{
	Ship* opponent = new Ship();
	opponent->displayHeading.set_degrees(180);
	if (shipType == 1)
		opponent->setSprite("Hulls\\drone");
	else if (shipType == 2)
		opponent->setSprite("Hulls\\Drone1");
	else if (shipType == 3)
		opponent->setSprite("Hulls\\Fighter1");
	
	opponent->projectileBrain = &makeBoringBrain;
		opponent->brain->addChild(new TurnTowardsTarget(1));
		opponent->brain->addChild(( new RepeatNode(-1))->addChild(
			(new SequentialNode())
				->addChild(new BoolCondition<GameObject>(&GameObject::alignedWithPlayer,true))
				->addChild(new Fire())
				->addChild(new Cooldown(500))));
		ProbabilityNode* probNode = new ProbabilityNode(); //get out of the way in a random direction when not near an edge
		probNode->addChild((
			new SequentialNode())
				->addChild(new AbsoluteMovement(LEFT,10,200)))
			->addChild((new SequentialNode())
				->addChild(new AbsoluteMovement(RIGHT,10,200)));

		opponent->brain
			->addChild((new RepeatNode(-1)) ->addChild((new SequentialNode())
			->addChild(new BoolCondition<GameObject>(&GameObject::playerIsAligned,true))
				->addChild(probNode)));
			

	ws->insertObject(opponent,point(100,100));
	return BT_SUCCESS;
}