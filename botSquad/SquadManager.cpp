#include "SquadManager.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
SquadManager::SquadManager(std::set<PUnit*> units)
	: target(nullptr)
	, brain(nullptr)
	, squadUnits(units)
{
}

	// Methods
void SquadManager::update()
{	
}
void SquadManager::addUnits(std::set<PUnit> newUnits)
{	
}
bool SquadManager::destroyUnit(Unit unit)
{	
	for(auto i=squadUnits.begin();i!=squadUnits.end();++i)
	{
		if((*i)->getID() == unit->getID())
		{
			squadUnits.erase(*i);
			return true;
		}
	}
	return false;
}
void SquadManager::setTarget(PositionOrUnit target)
{	
	positionToAttack = target;
}
bool SquadManager::isInPosition()
{
	return (positionToAttack.getApproxDistance(positionSquad) < 4);
}