#include "SquadManager.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
SquadManager::SquadManager(std::set<PUnit> units)
{
	squadUnits = units;
	positionToAttack = nullptr;
}

	// Methods

void SquadManager::update()
{	
}
void SquadManager::addUnits(std::set<PUnit> newUnits)
{	
}
void SquadManager::deleteUnits(std::set<PUnit> toRemoveUnits)
{	
}
void SquadManager::setTarget(BWAPI::Position target)
{	
	positionToAttack = target;
}