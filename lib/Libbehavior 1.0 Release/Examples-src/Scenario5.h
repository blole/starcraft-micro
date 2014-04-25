#include "Ship.h"
#include "GoStraight.h"
#include "AbsoluteMovement.h"
#include "DestroyAllNearbyProjectiles.h"
#include "Fire.h"
#include "Cooldown.h"
#include "WorldState.h"
#include "Projectile.h"
#include "turnTowardsTarget.h"
#include "BehaviorTree.h"
#include "BehaviorFactory.h"
#include "GotoPoint.h"

#ifndef SCENARIO5_H_
#define SCENARIO5_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario5()
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
		
		

		opponent->projectileBrain = &makeHomingBrain;
		opponent->brain
			->addChild((new SequentialNode())
				->addChild(new BoolCondition<GameObject>(&GameObject::projectileNearby,true))
				->addChild((new CountLimitNode(5,false))->addChild((new SequentialNode())
						->addChild(new DestroyAllNearbyProjectiles())
						->addChild(new Cooldown(2000)))));

		state->insertObject(opponent,point(100,50));
		return state;
	}


}
#endif