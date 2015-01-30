#include "Commander.hpp"
#include "SquadFire.hpp"
#include "MoveSquadToPosition.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;


		// Methods
void Commander::init()
{
	// Enemy units
	for (auto i : Broodwar->enemy()->getUnits())
	{
		if(i->getType()==UnitTypes::Terran_Marine)
		{
			OUnit* oUnit = OUnit::get(i);
			oAllUnits.insert(oUnit);
		}
	}

	// Create squad of 'nbUnitPerSquad' units
	int indexInSquad = 0;
	std::set<PUnit*> squadUnits;
	for (auto& i : Broodwar->self()->getUnits())
	{
		if(i->getType()==UnitTypes::Terran_Marine)
		{
			PUnit* pUnit = PUnit::get(i);
			if(indexInSquad >= nbUnitPerSquad)
			{
				indexInSquad=0;
				SquadManager* newSquad = new SquadManager();
				for(auto currentUnit = squadUnits.begin(); currentUnit!=squadUnits.end();currentUnit++)
					newSquad->addUnit(*currentUnit);
				squads.insert(newSquad);
				squadUnits.clear();
			}
			squadUnits.insert(pUnit);
			indexInSquad++;
		}
	}

	if(!squadUnits.empty()) // In case of a non full squad
	 {
	 	SquadManager* newSquad = new SquadManager();
	 	for(auto currentUnit = squadUnits.begin(); currentUnit!=squadUnits.end();currentUnit++)
	 		newSquad->addUnit(*currentUnit);
	 	squads.insert(newSquad);
	 }

	// Add a 'brain' to a squad
	for(auto& i : squads)
	{
		if(i->brain == nullptr)
			i->brain = (new ParallelNode())
							->addChild((new RepeatNode(-1))
								->addChild(new BoolCondition<SquadManager>(&SquadManager::isInPosition,true))
								->addChild(new SquadFire()))
							->addChild(new MoveSquadToPosition());
	}

	// My units
	for (auto& i : Broodwar->self()->getUnits())
	{
		PUnit* pUnit = PUnit::get(i);

		if(i->getType()==UnitTypes::Terran_Marine)
			pAllUnits.insert(pUnit);
	}
}

void Commander::update()
{
	if(oAllUnits.size()==0) // Winning scenario
		return;


	// TO IMPROVE
	auto squadIt = squads.begin();
	for(auto i=oAllUnits.begin(); i!=oAllUnits.end();++i)
	{
		if(squadIt == squads.end())
			break;

		if(!(*squadIt)->positionToAttack.getUnit()->exists())
			(*squadIt)->positionToAttack = (*i)->unit;
		
		++squadIt;
	}

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
			(*i)->removeUnit(PUnit::get(unit));
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