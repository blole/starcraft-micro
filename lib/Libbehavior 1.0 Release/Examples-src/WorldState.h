#ifndef WORLDSTATE_H_
#define WORLDSTATE_H_

#include <list>
#include <queue>
#include "GameObject.h"
#include "point.h"
#include <set>

namespace SL
{
class GameObject;
/// Convenience typedef for a standard list of GameObjects
typedef std::list<GameObject*> GameObjectList;
/// Convenience typedef for a GameObjectList iterator
typedef GameObjectList::iterator GameObjectIter;
/// Convenience typedef for a const GameObjectList iterator
typedef GameObjectList::const_iterator ConstGameObjectIter;
/// Determines how finely the world is broken down into cells which hold GameObjects
/// A larger value means larger cells, and thus lower memory usage
#define coarseGraining 10

/// Data structure that maps a collection of objects into a 2d space
class WorldState
{
public:

/// Width of a cell. Computed from the coarseGraining value
int CellSizeX;
/// Height of a cell. Computed from the coarseGraining value
int CellSizeY;
/// Width of the world
double CoordinateSizeX;
/// Height of the world
double CoordinateSizeY;
/// Current time in milliseconds. The absolute value doesn't mean anything, only differences
unsigned long long time;
/// Stores the amount of real time between frames for e.g. physics updates
unsigned int timeElapsed;
/// Description of the level
std::string description;
/// Name of the level
std::string name;
/// ID of the level
std::string id;

/// Standard Constructor
WorldState::WorldState();
/// Insert the gameObject into the world at the given point
/** \return true if the object was actually inserted, false if it could not be */
bool insertObject(GameObject* gameObject, point p);
/// Searches the world for an object with "isPlayer" set, and returns that
GameObject* getPlayer() const;
/// Marks the object for deletion. The memory is not actually freed until a convenient time, and until then the object will continue to interact with things (right now)
bool deleteObject(GameObject* gameObject);
/// Move an object that's already in the world to the specified point (checking for collisions)
/**	\return true if the object was actually moved, false if it collided, went out of bounds, etc */
bool moveObject(GameObject* gameObject, point p);
/// Rotate an object in the world about its center (checking for collisions)
/** \return true if the object was actually rotated, false if it collided, went out of bounds, etc */
bool rotateObject(GameObject* gameObject, float angle);
/// Get a list of all the objects in the region of a point
GameObjectList* getAtCell(point p);
/// Get a list of all the active gameObjects in the world
const GameObjectList* WorldState::getAllGameObjects() const;
/// Free the memory of objects that have been marked for deletion
void deleteQueued();
/// Get a list of all the inert gameObjects in the world
const GameObjectList* getBackgroundObjects();
/// Add an inert background object to the world. The object's location should already be set
void addBackgroundObject(GameObject* background);
/// Calculate the distance between obj1 and obj2. The distance is calculated based on their point locations, not on their full outlines.
float distanceBetween(const GameObject* obj1,const GameObject* obj2) const;
/// Calculate the angle from obj1 to obj2. The angles are in the range -180 to 180.
CL_Angle angleBetween(const GameObject* obj1,const GameObject* obj2) const;

private:
	CL_Sprite* background;
	GameObjectList*** worldMatrix;
	GameObjectList* activeObjectList;
	GameObjectList* backgroundObjectList;
	bool pointOutofBounds(point p);
	GameObjectList* WorldState::getListFromPoint(point p);
	GameObjectList* deleteList;
	void registerForDeletion(GameObject* obj);
	
};

}
#endif