#include "Ship.h"
#include "WorldState.h"
#include "Projectile.h"
#include "BehaviorTree.h"
#include "BehaviorFactory.h"

#ifndef SCENARIO2_H_
#define SCENARIO2_H_
using namespace SL;
using namespace SL::Behaviors;
namespace SL
{
	WorldState* loadScenario2()
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
			->addChild((new PriorityNode())
				->addChild((new SequentialNode())
					->addChild(new IntCondition<GameObject>(&GameObject::getHealth,GREATER_THAN,50))
					->addChild(new Fire())
					->addChild(new Cooldown(1000)))
				->addChild(makeHomingBrain()));
		state->insertObject(opponent,point(400,50));
		return state;
	}


}
#endif