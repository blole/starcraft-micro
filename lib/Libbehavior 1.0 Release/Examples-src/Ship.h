#ifndef SHIP_H_
#define SHIP_H_
#include <list>
#include <vector>
#include <string>
#include "point.h"
#include <set>
#include "GameObject.h"
#include "WorldState.h"
#include "Projectile.h"
#include "BehaviorTree.h"

class CL_ResourceManager;
using namespace BehaviorTree;

namespace SL
{
class WorldState;

class Ship: public GameObject {

public:
	
	bool doActions();
	bool registerCollision(GameObject* collidedObject);
	bool registerWallCollision();
	Ship::Ship();
	void Ship::fire(Projectile* p);
	BehaviorTreeInternalNode* (*projectileBrain)();
};

}
#endif