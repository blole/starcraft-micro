#include "SquadManager.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

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
}

void SquadManager::setTarget(PositionOrUnit target)
{	
	positionToAttack = target;
}
bool SquadManager::isInPosition()
{
	return (positionToAttack.getPosition().getApproxDistance(positionSquad) < 4);
}