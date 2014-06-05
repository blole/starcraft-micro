#include "Commander.hpp"
#include "behaviors/Flee.hpp"
#include "behaviors/AttackClosest.hpp"
#include "behaviors/AttackClosestLethal.hpp"
#include "behaviors/Attack.hpp"
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
		if(i->getType()!=UnitTypes::Special_Map_Revealer)
		{
			OUnit* oUnit = OUnit::get(*i);
			oAllUnits.insert(oUnit);
		}
	}

	// My units
	Unitset tmpUnit = Broodwar->self()->getUnits();
	for(auto i=tmpUnit.begin();i!=tmpUnit.end();++i)
	{
		if(i->getType()!=UnitTypes::Special_Map_Revealer)
		{
		PUnit* pUnit = PUnit::get(*i);

		if(i->getType()!=UnitTypes::Special_Map_Revealer)
			pAllUnits.insert(pUnit);

		if (pUnit->brain == nullptr)
			pUnit->brain=(new SequentialNode())
				->addChild(new Flee())
				//->addChild(new AcquireTarget())
				->addChild(new Attack());
		//	//pUnit->brain = (new ParallelNode())
		//	//	->addChild((new SequentialNode())
		//	//		->addChild(new BoolCondition<PUnit>(&PUnit::isUnderAttack,true))
		//	//		->addChild(new Flee()))
		//	//	->addChild((new SequentialNode())
		//	//		->addChild(new BoolCondition<PUnit>(&PUnit::isUnderAttack,false))
		//	//		->addChild(new AttackClosest()))
		//	pUnit->brain = (new SequentialNode())
		//		->addChild(new AttackClosestLethal())
		//		->addChild(new Flee());
		}
	}

	influenceMap.init();
}

void Commander::update()
{

	if(oAllUnits.size()==0) // Winning scenario
		return;
	
	influenceMap.update();

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