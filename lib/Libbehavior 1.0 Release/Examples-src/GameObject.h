#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_
#include <list>
#include <string>
#include "point.h"
#include <set>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/application.h>
#include "globals.h"
#include "BehaviorTree.h"

class CL_Sprite;
class CL_CollisionOutline;

namespace SL
{
class WorldState;
class BehaviorTreeNode;
class GameObject;

/// Base class for any item in the game
class GameObject{

public:
	/// Called every frame to update the object
	virtual bool doActions();
	/// Notify the object of a collision with another object. Only called if usesPhysics is true
	virtual bool registerCollision(GameObject* collidedObject);
	/// Notify the object it has hit the edge of the level. Responding improperly to this can cause out of bounds errors
	virtual bool registerWallCollision();
	/// Set the GameObject's sprite, and load the collision outline if it exists. If it doesn't, regenerate and save it.
	//* The resource is a relative filename to an image (without the extension). e.g. "Ammo\\test"
	void GameObject::setSprite(std::string resource);
	/// Default constructor
	GameObject::GameObject();
	/// The graphical representation of the object
	CL_Sprite* sprite;
	/// Cleans up resources in use by the object when it is deleted
	GameObject::~GameObject();
	/// If Newtonian physics is enabled for the world, this imparts a force on the object
	void applyForceRect(float x, float y);
	/// If Newtonian physics is enabled for the world, this imparts a force on the object
	void applyForcePolar(CL_Angle heading, float magnitude);
	float getXPosition() const
	{
		return location.x;
	}
	float getYPosition() const
	{
		return location.y;
	}
	/// The "AI" for the object.
	BehaviorTree::BehaviorTreeInternalNode* brain;
	/// Unique for each object instance
	int ID;
	/// Generate a unique ID for an object
	static int GameObject::getID()
{
	IDCount++;
	return IDCount;
}

	/// The human friendly name of the object
	std::string displayName; 
	/// True if this object is the player. Undefined behavior if more than one object has this flag set to true.
	bool isPlayer;
	
	/// The number of "hit points" the object has.
	unsigned int HP;
	/// The directional component of the object's velocity
	CL_Angle movementHeading;
	/// The way the object's sprite is "turned"
	CL_Angle displayHeading;
	/// The collision outline generated from the sprite
	CL_CollisionOutline* collisionOutline;
	/// The directional component of the object's acceleration
	float accelHeading;
	/// The scalar component of the object's acceleration
	float accelMagnitude;
	/// Used to divide objects into different categories to customize interactions between them
	int faction;
	/// Used to scale how objects are displayed. Not currently implemented
	int displaySize;
	/// Absolute coordinates of the object in the world.
	point location;
	/// The scalar component of the object's velocity
	float speed;
	/// Determines whether the object collides with others
	bool usesPhysics;
	/// Number of milliseconds the object has been in existence. Updated every tick based on the timeElapsed
	long age;
	/// Am I facing the player
	bool alignedWithPlayer() const;
	/// Is the player facing me
	bool playerIsAligned() const;
	GameObject* owner; //Generally the object that created this object, if any
	int getHealth() const;
	bool projectileNearby() const;
	float distanceToPlayer() const;

 protected:
	
	 bool processMovementPhysics();
	 void loadCollisionOutline(std::string source,CL_PixelBuffer image);
 private:
	
	 static int IDCount;
	 

};
}
#endif