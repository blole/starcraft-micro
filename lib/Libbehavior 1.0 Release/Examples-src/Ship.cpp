#include "Ship.h"
#include "Definitions.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Projectile.h"

using namespace SL;
using namespace std;

bool Ship::doActions()
{

	//move();
	//shoot();
	if (!isPlayer)
		brain->execute(this);
	return true;
	
}

///Sets the projectile heading and inserts it into the world
void Ship::fire(Projectile* p)
{
	p->displayHeading = *(new CL_Angle(displayHeading));
	ws->insertObject(p,point(location.x + sprite->get_width()/2,location.y + sprite->get_height()/2)); //TODO: insert the projectile at a more suitable point
}

Ship::Ship() : GameObject()
{
	GameObject::usesPhysics = true;
}
	
bool Ship::registerWallCollision()
{
	//cancel accelerations
	accelMagnitude = 0;
	speed = 0;

	
	return true;
}

bool Ship::registerCollision(GameObject* collidedObject)
 {
	cout << "ship collision" << endl;
	if (HP <= 50 && !isPlayer)
		ws->deleteObject(this);
	else
		HP -= 50;
	return true;
}

