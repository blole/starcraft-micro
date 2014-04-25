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

#ifndef SCENARIO6_H_
#define SCENARIO6_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario6()
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


		for (int i = 0 ; i< 55 ; i++)
		{
			Ship* opponent = new Ship();
			opponent->displayHeading.set_degrees(180);
			opponent->setSprite("Hulls\\drone");
			opponent->HP = 50;
			

			opponent->projectileBrain = &makeSlowBoringBrain;
			opponent->brain->addChild((new RepeatNode(-1))->addChild((new SequentialNode())
					->addChild(new AbsoluteMovement(RIGHT,.5,2000))
					->addChild(new AbsoluteMovement(DOWN,.5,1000))
					->addChild(new AbsoluteMovement(LEFT,.5,2000))
					->addChild(new AbsoluteMovement(DOWN,.5,1000))));
			
			ProbabilityNode* firing = new ProbabilityNode();
			firing->addChild(new Fire(),1);
			firing->addChild(new Cooldown(100),100);
			opponent->brain->addChild((new RepeatNode(-1))->addChild(firing));
				

			state->insertObject(opponent,point(50 + 50 * (i%11),50 + (50 * (i/11))));
		}
		return state;
	}


}
#endif