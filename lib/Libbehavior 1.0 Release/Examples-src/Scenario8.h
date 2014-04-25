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




#ifndef SCENARIO8_H_
#define SCENARIO8_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{

	int getPlayerHealth()
	{
		return ws->getPlayer()->getHealth();
	}


	WorldState* loadScenario8()
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

		GameObject* director = new GameObject();

		director->brain->addChild(
			(new SequentialNode())
			->addChild(new SpawnShip(1))
			->addChild(new WaitForNextWave())
			->addChild(new SpawnShip(2))
			->addChild(new WaitForNextWave())
			->addChild(new IntCondition<>(getPlayerHealth,EQUAL,100)) //only spawn the "secret" enemy if the player didn't get hit fighting the other two
			->addChild(new SpawnShip(3))
			->addChild(new WaitForNextWave()));

		state->insertObject(director,point(0,0));

		return state;
	}

	


}
#endif