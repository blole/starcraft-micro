#include "SquadManager.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
SquadManager::SquadManager(std::set<PUnit*> units)
	: positionToAttack(nullptr)
	, brain(nullptr)
{
	for(auto i=units.begin();i!=units.end();i++)
	{
		squadUnits.insert(*i);
	}
}

	// Methods
void SquadManager::update()
{	
}
void SquadManager::addUnit(PUnit* unit)
{	
	squadUnits.insert(unit);
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
	return (positionToAttack.getPosition().getApproxDistance(positionSquad) < 4);
}