#include "Commander.hpp"
#include "behaviors/AttackClosest.hpp"
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
			OUnit* oUnit = new OUnit(*i);
			oAllUnits.insert(oUnit);
		}
	}

	// Create squad of 'nbUnitPerSquad' units
	Unitset allGameUnit = Broodwar->self()->getUnits();
	bool indexInSquad = 0;
	std::set<PUnit*> squadUnits;
	for(auto i=tmpUnit.begin();i!=tmpUnit.end();++i)
	{
		PUnit* pUnit = PUnit::get(*i);
		if(indexInSquad >= nbUnitPerSquad)
		{
			indexInSquad=0;
			squads.insert(new SquadManager(squadUnits));
			squadUnits.clear();
		}
		squadUnits.insert(pUnit);
		indexInSquad++;
	}
	if(!squadUnits.isEmpty()) // In case of a non full squad
		squads.insert(new SquadManager(squadUnits));

	// Add a 'brain' to a squad
	for(auto i=squads.begin(); i!=squads.end();++i)
	{
		if((*i)->brain == nullptr)
			(*i)->brain = (new ParrallelNode())
							->addChild((new RepeadNode(-1))
								->addChild(new BoolCondition<SquadManager>(&SquadManager::isInPosition,true))
								->addChild(new SquadFire()))
							->addChild(new MoveSquadToPosition());
	}

	// My units
	Unitset tmpUnit = Broodwar->self()->getUnits();
	for(auto i=tmpUnit.begin();i!=tmpUnit.end();++i)
	{
		PUnit* pUnit = PUnit::get(*i);

		if(i->getType()==UnitTypes::Terran_Marine)
			pAllUnits.insert(pUnit);
	}
}

void Commander::update()
{
	if(oAllUnits.size()==0) // Winning scenario
		return;

	for(auto i=squads.begin(); i!=squads.end();++i)
	{
		(*i)->brain->execute(*i);
	}

	//for(auto i=pAllUnits.begin();i!=pAllUnits.end();i++)
	//{
	//		(*i)->brain->execute(*i);
	//}
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

		for(auto i=squads.begin();i!=squads.end();i++)
		{
			if((*i)->destroyUnit(unit))
				break;
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