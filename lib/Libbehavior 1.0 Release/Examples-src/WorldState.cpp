#include "WorldState.h"
#include "GameObject.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "Projectile.h"
#include "util.h"
#include "Ship.h"

using namespace SL;
using namespace std;

WorldState::WorldState()
{
	description = "";
	CoordinateSizeX = 640.0;
	CoordinateSizeY = 480.0;

	CellSizeX = (int)CoordinateSizeX / coarseGraining ;
	CellSizeY = (int)CoordinateSizeY / coarseGraining ;

	activeObjectList = new GameObjectList();
	deleteList = new GameObjectList();
	backgroundObjectList = new GameObjectList();
	worldMatrix = new GameObjectList**[CellSizeX];
	for (int i = 0; i < CellSizeX; ++i)
        worldMatrix[i] = new GameObjectList*[CellSizeY];

	for (int i=0;i<CellSizeX;i++)
	{
		for (int j=0;j<CellSizeY;j++)
		{
			worldMatrix[i][j] = new GameObjectList();
		}
	}
	time = CL_System::get_time();
}

GameObject* WorldState::getPlayer() const
{
	GameObjectIter itr;
	for (itr = activeObjectList->begin();itr != activeObjectList->end(); itr++)
	{
		if ((*itr)->isPlayer)
			return *itr;
	}
	throw std::exception("no camera defined");
	return NULL;
}

bool WorldState::insertObject(GameObject* gameObject, point p)
{
	if (pointOutofBounds(p))
		return false;

	GameObjectList* currentList = getListFromPoint(p);
	currentList->push_front(gameObject);
	gameObject->location = p;
	activeObjectList->push_front(gameObject);
	if (gameObject->usesPhysics)
		gameObject->collisionOutline->set_translation(p.x,p.y);
	return true;
}

bool WorldState::deleteObject(GameObject* gameObject)
{
	registerForDeletion(gameObject);
	return true;
}

bool WorldState::moveObject(GameObject* gameObject, point p)
{
	if (pointOutofBounds(p))
	{
		gameObject->registerWallCollision();
		return false;
	}

	//test for and report potential collisions
	//currently tests against all objects (does not scale well)
	//update with more sophisticated filtering in the future

	if (gameObject->usesPhysics)
		gameObject->collisionOutline->set_translation(p.x,p.y);
	GameObjectIter itr;
	if (gameObject->usesPhysics)
	{
		for (itr = activeObjectList->begin();itr != activeObjectList->end(); itr++)
		{
			if ((*itr) == gameObject)
				continue;
			if ((*itr)->faction == PROJECTILE_FACTION && (*itr)->owner != ws->getPlayer() &&  (gameObject->isPlayer == false))
				continue;
			if (gameObject->faction == PROJECTILE_FACTION && gameObject->owner != ws->getPlayer() && ((*itr)->isPlayer == false))
				continue;
			
			//if (((*itr)->owner == gameObject)|| (gameObject->owner == *itr))
			//	continue;

			if ((*itr)->usesPhysics)
			{
				if ((*itr)->collisionOutline->collide(*(gameObject->collisionOutline)))
				{
					cout << (*itr)->displayName <<endl;
					cout << gameObject->displayName << endl;
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);
					gameObject->collisionOutline->set_translation(gameObject->location.x,gameObject->location.y);
					return false; //TODO support colliding with multiple entities
				}
			}

		}
	}


	GameObjectList* currentList = getListFromPoint(gameObject->location);
		GameObjectList* newList = getListFromPoint(p);
		gameObject->location=p;
		if (currentList==newList)
			{}
		else
		{
			newList->push_front(gameObject);
		currentList->remove(gameObject);
		}
		gameObject->collisionOutline->set_translation(p.x,p.y);
		return true;

}

bool WorldState::rotateObject(GameObject* gameObject, float angle)
{
	if (gameObject->usesPhysics)
	{
		gameObject->collisionOutline->rotate(CL_Angle::from_degrees(angle));

		GameObjectIter itr;
 		for (itr = activeObjectList->begin();itr != activeObjectList->end(); itr++)
		{
			if ((*itr) == gameObject)
				continue;
			if ((*itr)->faction == PROJECTILE_FACTION && (gameObject->faction == PROJECTILE_FACTION))
				continue;
			if (((*itr)->owner == gameObject)|| (gameObject->owner == *itr))
				continue;
			if ((*itr)->usesPhysics)
			{
				if ((*itr)->collisionOutline->collide(*(gameObject->collisionOutline)))
				{
					(*itr)->registerCollision(gameObject);
					gameObject->registerCollision(*itr);
					gameObject->collisionOutline->set_angle(gameObject->displayHeading);
					return false; //TODO support colliding with multiple entities
				}
			}
		}
	}

	gameObject->displayHeading += CL_Angle::from_degrees(angle);
	float newAngle = fmod(gameObject->displayHeading.to_degrees(),360.0f);
	gameObject->displayHeading.set_degrees(newAngle);
	return true;
}

void WorldState::registerForDeletion(GameObject* obj)
{
	deleteList->push_front(obj);
}

void WorldState::deleteQueued()
{
	ConstGameObjectIter itr;
	deleteList->sort();
	deleteList->unique();
	for(itr = deleteList->begin();itr !=deleteList->end();)
	{
		GameObjectList* currentList = getListFromPoint((*itr)->location);
		activeObjectList->remove((*itr));
		currentList->remove((*itr));
		delete (*itr++);
	}
	deleteList->clear();
}

void WorldState::addBackgroundObject(GameObject* background)
{
	backgroundObjectList->push_back(background);
}

const GameObjectList* WorldState::getBackgroundObjects()
{
	return backgroundObjectList;
}

GameObjectList* WorldState::getAtCell(point p)
{
	if (pointOutofBounds(p))
		return NULL;
	return getListFromPoint(p);
}

const GameObjectList* WorldState::getAllGameObjects() const
{
	return  activeObjectList; 
}


bool WorldState::pointOutofBounds(point p)
{
	if (p.x >= CoordinateSizeX || p.y >= CoordinateSizeY || p.x < 0 || p.y < 0)
		return true;
	return false;

}

GameObjectList* WorldState::getListFromPoint(point p)
{
	return worldMatrix[(int)p.x / coarseGraining][(int)p.y / coarseGraining];
}

float SL::WorldState::distanceBetween(const GameObject* obj1,const GameObject* obj2 ) const
{
	point resultantVector(obj1->location.x - obj2->location.x,obj1->location.y - obj2->location.y);
	return resultantVector.vectorSize();
}

CL_Angle SL::WorldState::angleBetween( const GameObject* obj1,const GameObject* obj2 ) const
{
	point resultantVector(obj1->location.x - obj2->location.x,obj1->location.y - obj2->location.y);
	resultantVector.normalize();
	CL_Angle angle = CL_Angle::from_radians(atan2(resultantVector.y,resultantVector.x));
	CL_Angle angleBetween = angle - obj1->displayHeading + CL_Angle::from_degrees(-90.0f);
	angleBetween = CL_Angle::from_degrees(mod(angleBetween.to_degrees(),360.0f)); 
	if (angleBetween > CL_Angle::from_degrees(180.0f))
		angleBetween = CL_Angle::from_degrees(180.0f) - angleBetween;
	return angleBetween;
}