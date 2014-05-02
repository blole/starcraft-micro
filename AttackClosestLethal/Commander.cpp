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
	Unitset tmpEnemyUnit = Broodwar->enemy()->getUnits();
	for(auto i=tmpEnemyUnit.begin();i!=tmpEnemyUnit.end();++i)
	{
		if(i->getType()==UnitTypes::Terran_Marine)
		{
			OUnit* oUnit = OUnit::get(*i);
			oAllUnits.insert(oUnit);
		}
	}

	// My units
	Unitset tmpUnit = Broodwar->self()->getUnits();
	for(auto i=tmpUnit.begin();i!=tmpUnit.end();++i)
	{
		PUnit* pUnit = PUnit::get(*i);

		if(i->getType()==UnitTypes::Terran_Marine)
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