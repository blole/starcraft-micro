#include "main-kiting-squad/squadmanager.hpp"

using namespace Bot;
using namespace BWAPI;

SquadManager::SquadManager()
	: brain(nullptr)
{
}

void SquadManager::onFrame()
{	
	Position newSquadPosition(0,0);
	for (auto& unit : units)
		newSquadPosition += unit->getPosition();

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