#include "bot/commander.hpp"
#include "behaviors/moverelative.hpp"

using namespace BWAPI;

Commander::Commander()
{
	brain = (new SequentialNode())
		->addChild(new MoveRelative(10,0))
		->addChild(new MoveRelative(-10,0));
}

void Commander::onFrame()
{
	static bool first = true;
	static Unit unit;
	if (first)
	{
		first = false;
		unit = Broodwar->self()->getUnits()[1];
		brain->init(unit);
	}
	
	brain->execute(unit);
}
