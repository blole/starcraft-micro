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
#include "SelfDestruct.h"

#ifndef SCENARIO7_H_
#define SCENARIO7_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	bool friendsAreAround()
	{
		int count = 0;
		ConstGameObjectIter itr;
		const GameObjectList* objects = ws->getAllGameObjects();
		for (itr = objects->begin();itr != objects->end(); itr++)
		{
			if ((*itr)->isPlayer == false && (*itr)->faction == PROJECTILE_FACTION == false)
				count++;
		}
		return count >= 2;
	}
	WorldState* loadScenario7()
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
			opponent->setSprite("Hulls\\drone");
			opponent->HP = 50;
			

			opponent->projectileBrain = &makeBoringBrain;
			opponent->brain
			->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
					->addChild(new BoolCondition<>(&friendsAreAround,true))
					->addChild(new Fire())
					->addChild(new AbsoluteMovement(RIGHT,10,500))
					->addChild(new AbsoluteMovement(LEFT,10,500))));
				

			state->insertObject(opponent,point(100,100));

			opponent = new Ship();
			opponent->displayHeading.set_degrees(180);
			opponent->setSprite("Hulls\\drone");
			

			opponent->projectileBrain = &makeBoringBrain;
			opponent->brain
			->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
					->addChild(new BoolCondition<>(&friendsAreAround,true))
					->addChild(new Fire())
					->addChild(new AbsoluteMovement(LEFT,10,500))
					->addChild(new AbsoluteMovement(RIGHT,10,500))));
				

			state->insertObject(opponent,point(100,150));

			opponent = new Ship();
			opponent->displayHeading.set_degrees(180);
			opponent->setSprite("Hulls\\drone");
			

			opponent->projectileBrain = &makeBoringBrain;
			opponent->brain
			->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
				->addChild(new BoolCondition<>(&friendsAreAround,true))
				->addChild(new AbsoluteMovement(RIGHT,10,500))
				->addChild(new Fire())
				->addChild(new AbsoluteMovement(LEFT,10,500))));
					
				
			state->insertObject(opponent,point(100,200));

			opponent = new Ship();
			opponent->displayHeading.set_degrees(180);
			opponent->setSprite("Hulls\\drone");
			

			opponent->projectileBrain = &makeBoringBrain;
			opponent->brain
			->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
					->addChild(new BoolCondition<>(&friendsAreAround,true))
					->addChild(new AbsoluteMovement(LEFT,10,500))
					->addChild(new AbsoluteMovement(RIGHT,10,500))
					->addChild(new Fire())));
				
				

			state->insertObject(opponent,point(100,250));
		return state;
	}


}
#endif