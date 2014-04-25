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

#ifndef SCENARIO4_H_
#define SCENARIO4_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario4()
	{
		WorldState* state = new WorldState();

		GameObject* background = new GameObject();
		background->usesPhysics = false;
		background->setSprite("Backgrounds\\background");
		state->addBackgroundObject(background);

		Ship* player = new Ship();
		player->setSprite("Hulls\\Sample Hull");
		player->isPlayer = true;
		player->projectileBrain = &makeBoringBrain;
		state->insertObject(player,point(100,300));

		Ship* opponent = new Ship();
		opponent->displayHeading.set_degrees(180);
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
			
		state->insertObject(opponent,point(100,50));
		return state;
	}


}
#endif