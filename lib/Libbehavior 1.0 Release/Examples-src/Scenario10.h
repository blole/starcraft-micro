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
#include "SpawnShip.h"
#include "WaitForNextWave.h"




#ifndef SCENARIO10_H_
#define SCENARIO10_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	void decloak(void* ship)
	{
		Ship* s = (Ship*) ship;
		s->setSprite("Hulls\\Sample Hull");
	}
	WorldState* loadScenario10()
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


			Ship* enemy = new Ship();
		enemy->setSprite("Hulls\\Sample Hull Cloaked");
		enemy->projectileBrain = &makeBoringBrain;

		enemy->brain = new ParallelNode();
		enemy->brain->addChild(
			new TurnTowardsTarget(2));
		enemy->brain->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
			->addChild(new Fire())
			->addChild(new Cooldown(1000))));
		enemy->brain->addChild((new SequentialNode())
			->addChild(new FloatCondition<GameObject>(&GameObject::distanceToPlayer,LESS_THAN_FP,200))
			->addChild((new CountLimitNode(1,false))
				->addChild(new FunctionCall<>(&decloak,(void*)enemy))));
		state->insertObject(enemy,point(50,100));



		return state;
	}

	


}
#endif