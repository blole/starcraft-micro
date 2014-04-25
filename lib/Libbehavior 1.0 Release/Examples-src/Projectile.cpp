#include "Projectile.h"
#include "globals.h"

using namespace SL;

bool Projectile::doActions()
{

	if (brain != NULL)
	{
		brain->execute(this);
	}
	return true;
	
}
/*
bool Projectile::move()
{
	#ifndef PHYSICS
	cout<< "projectile update" << endl;
	ws->moveObject(this,location.offsetPolar(displayHeading,20));
	#endif
	return true;
}*/

Projectile::Projectile() : GameObject()
{
	displayName = "projectile";
	faction = PROJECTILE_FACTION;
}
	
bool Projectile::registerWallCollision()
{
	cout << "projectile wall collision" << endl;
	ws->deleteObject(this);
	return true;
}

bool Projectile::registerCollision(GameObject* collidedObject)
{
	cout << "projectile collision" << endl;
	ws->deleteObject(this);
	return true;
}
