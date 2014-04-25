#include "Fire.h"
#include "point.h"
#include "GameObject.h"
#include "worldstate.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Ship.h"
#include <iostream>
using namespace SL;
using namespace SL::Behaviors;
using namespace BehaviorTree;
using namespace std;

void Fire::init(void* agent)
{
};

Fire::Fire()
{
}
BEHAVIOR_STATUS Fire::execute(void* agent)
{
	cout << "Fire!" << endl;
	Ship* ship = (Ship*) agent;
	Projectile* p = new Projectile();
	p->setSprite("Ammo\\test");
	p->brain = ship->projectileBrain();
	p->owner = ship;
	ship->fire(p);
	
	return BT_SUCCESS;
}