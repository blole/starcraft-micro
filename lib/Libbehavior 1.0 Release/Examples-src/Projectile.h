#ifndef PROJECTILE_H_
#define PROJECTILE_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include "GameObject.h"
#include "Definitions.h"


namespace SL
{
class WorldState;
class GameObject;

#define PROJECTILE_FACTION 500

class Projectile: public GameObject {

public:

	bool doActions();
	bool registerCollision(GameObject* collidedObject);
	bool registerWallCollision();
	Projectile::Projectile();
	

};

}
#endif