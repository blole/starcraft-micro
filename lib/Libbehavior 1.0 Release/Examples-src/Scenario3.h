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

#ifndef SCENARIO3_H_
#define SCENARIO3_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario3()
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
		ProbabilityNode* pNode = new ProbabilityNode();
		pNode->addChild((new SequentialNode())
				->addChild(new BoolCondition<GameObject>(&GameObject::alignedWithPlayer,true))
				->addChild(new Fire())
				->addChild(new Cooldown(500)),5.0);
		pNode->addChild((
			new SequentialNode())
				->addChild(new FloatCondition<GameObject>(&Ship::getXPosition,LESS_THAN_FP,200,1))
				->addChild(new GotoPoint(point(300,50),500))
				->addChild(new Cooldown(200)))
			->addChild((new SequentialNode())
				->addChild(new FloatCondition<GameObject>(&Ship::getXPosition,GREATER_OR_CLOSE,200,1))
				->addChild(new GotoPoint(point(100,50),500))
				->addChild(new Cooldown(200)));
		opponent->brain
			->addChild((new RepeatNode(-1))
				->addChild(pNode));
			
		state->insertObject(opponent,point(100,50));
		return state;
	}


}
#endif