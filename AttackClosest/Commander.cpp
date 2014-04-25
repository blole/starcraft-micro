#include "Commander.hpp"
#include "behaviors/AttackClosest.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
Commander::Commander() {
	brain = (new SequentialNode())->addChild(new AttackClosest());
}

		// Methods
void Commander::update()
{
	static bool first = true;
	static std::set<PUnit*> units;
	if (first)
	{
		first = false;
		Unitset tmpUnit = Broodwar->self()->getUnits();
		for(auto i=tmpUnit.begin();i!=tmpUnit.end();i++) 
		{
			PUnit* pUnit = new PUnit(*i);
			brain->init(pUnit);
			units.insert(pUnit);
		}
	}
	
	for(auto i=units.begin();i!=units.end();i++)
	{
		brain->execute(*i);
	}
}
void Commander::destroyUnit(GameUnit unit)
{
	
}