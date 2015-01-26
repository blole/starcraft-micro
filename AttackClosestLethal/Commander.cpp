#include "Commander.hpp"
#include "behaviors/AttackClosestLethal.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
Commander::Commander()
{
}

		// Methods
void Commander::init()
{
	// Enemy units
	for (auto& i : Broodwar->enemy()->getUnits())
	{
		if(i->getType()!=UnitTypes::Special_Map_Revealer)
		{
			OUnit* oUnit = OUnit::get(i);
			oAllUnits.insert(oUnit);
		}
	}

	// My units
	for (auto& i : Broodwar->self()->getUnits())
	{
		PUnit* pUnit = PUnit::get(i);

		if(i->getType()!=UnitTypes::Special_Map_Revealer)
			pAllUnits.insert(pUnit);

		if (pUnit->brain == nullptr)
			pUnit->brain = (new SequentialNode())
				->addChild(new AttackClosestLethal());
	}
}

void Commander::update()
{
	if(oAllUnits.size()==0) // Winning scenario
		return;

	for(auto i=pAllUnits.begin();i!=pAllUnits.end();i++)
	{
			(*i)->brain->execute(*i);
	}
}
void Commander::destroyUnit(BWAPI::Unit unit)
{
	// If its one of my units
	if(unit->getPlayer()->getID() == Broodwar->self()->getID())
	{
		for(auto i=pAllUnits.begin();i!=pAllUnits.end();i++)
		{
			if((*i)->getID() == unit->getID())
			{
				pAllUnits.erase(i);
				break;
			}
		}
	}
	else // If its one of my ennemy
	{
		for(auto i=oAllUnits.begin();i!=oAllUnits.end();i++)
		{
			if((*i)->getID() == unit->getID())
			{
				oAllUnits.erase(i);
				break;
			}
		}
	}
}