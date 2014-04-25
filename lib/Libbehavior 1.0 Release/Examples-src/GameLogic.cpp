#include "GameLogic.h"

#include "GameObject.h"
#include "Ship.h"
#include "Definitions.h"
#include "globals.h"
#include "Projectile.h"

using namespace SL;
using namespace std;

GameLogic::GameLogic()
{
	keyboard = &ic->get_keyboard();
	mouse = &ic->get_mouse();
	playerShip = (Ship*) ws->getPlayer();
}

bool GameLogic::step()
{
	
	ws->deleteQueued(); // Delete all the things that were destroyed last frame now, since it won't interfere with iterators, etc.
	unsigned int currentTime = CL_System::get_time(); //Since the framerate might not be steady, need to see how much actual time has gone by
	ws->timeElapsed = currentTime - ws->time;
	ws->time = currentTime;

	//should handle input with event callbacks, but I can't get them to work
	handleInput();
	
	const GameObjectList* objects = ws->getAllGameObjects();
	ConstGameObjectIter itr;
	for(itr = objects->begin();itr !=objects->end();)
	{
#ifdef PHYSICS
		(*itr)->processMovementPhysics();
#endif
		(*itr)->age += ws->timeElapsed;
 		(*itr++)->doActions();
	}

	return true;
}

void GameLogic::handleInput()
{
	static int timeLastFired;
	 if (keyboard->get_keycode(CL_KEY_SPACE))
	 {
		 
		 if (ws->time - timeLastFired > 100)
		 {
			 Projectile* p = new Projectile();
			 p->setSprite("Ammo\\test");
			p->displayHeading = *(new CL_Angle(playerShip->displayHeading));
			p->brain = playerShip->projectileBrain();
			p->owner = playerShip;
			ws->insertObject(p,playerShip->location.offsetPolar(playerShip->displayHeading,0));
		 }
		 timeLastFired = ws->time;
	 }


#ifdef ABSOLUTE_MOVEMENT
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetRect(0,-10));
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetRect(0,10));
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		ws->moveObject(playerShip,playerShip->location.offsetRect(-10,0));
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		ws->moveObject(playerShip,playerShip->location.offsetRect(10,0));
	 }
#endif

#ifdef RELATIVE_MOVEMENT
#ifdef NOPHYSICS
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetPolar(playerShip->displayHeading,5));
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 ws->moveObject(playerShip,playerShip->location.offsetPolar(playerShip->displayHeading,-5));
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		 ws->rotateObject(playerShip,-5);
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		 ws->rotateObject(playerShip,5);
	 }
#endif
#ifdef PHYSICS
	 if (keyboard->get_keycode(CL_KEY_UP))
	 {
		 playerShip->applyForcePolar(playerShip->displayHeading,.005);
	 }

	 if (keyboard->get_keycode(CL_KEY_DOWN))
	 {
		 playerShip->applyForcePolar(playerShip->displayHeading,-.005);
	 }

	 if (keyboard->get_keycode(CL_KEY_LEFT))
	 {
		 playerShip->rotate(-2);
	 }

	 if (keyboard->get_keycode(CL_KEY_RIGHT))
	 {
		 playerShip->rotate(2);
	 }
#endif
#endif


	 if (keyboard->get_keycode(CL_KEY_ESCAPE))
	 {
		exit(0);
	 }

	  if (keyboard->get_keycode(CL_KEY_LSHIFT))
	  {
		  playerShip->accelMagnitude = 0;
		  playerShip->speed = 0;
	  }

}