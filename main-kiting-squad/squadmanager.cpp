#include "squadmanager.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
using namespace Bot;

	// Constructor
SquadManager::SquadManager()
	: Squad()
	, positionToAttack(nullptr)
	, brain(nullptr)
{
}

	// Methods
void SquadManager::onFrame()
{	
	Position newSquadPosition(0,0);
	for(auto i=units.begin();i!=units.end();i++)
	{
		newSquadPosition += (*i)->getPosition();
	}
	newSquadPosition /= (units.size());
	this->positionSquad = newSquadPosition;

	//// DEBUG DISPLAY
	//displaySquadLinks();
}

void SquadManager::setTarget(PositionOrUnit target)
{	
	positionToAttack = target;
}
bool SquadManager::isInPosition()
{
	return (positionToAttack.getPosition().getApproxDistance(positionSquad) < 100);
}