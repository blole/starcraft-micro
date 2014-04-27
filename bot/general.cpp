#include "bot/general.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

General::General()
{
}

void General::onStart()
{
	
}

void General::onFrame()
{
	Unitset myUnits = Broodwar->self()->getUnits();
	
	for (Unitset::iterator u = myUnits.begin(); u != myUnits.end(); ++u)
	{
		if (!u->exists()		|| u->isLockedDown()	||
			u->isMaelstrommed()	|| u->isStasised()		||
			u->isLoaded()		|| !u->isPowered()		||
			u->isStuck()		|| !u->isCompleted()	||
			u->isConstructing())
			continue;

		PUnit* pUnit = PUnit::get(*u);

		if (pUnit->brain == nullptr)
		{
			pUnit->brain =
				(new SequentialNode())
					->addChild((new BehaviorTree::RepeatNode(30))
						->addChild(new MoveRelative(1, 0)))
					->addChild((new BehaviorTree::RepeatNode(30))
						->addChild(new MoveRelative(0, 1)))
					->addChild((new BehaviorTree::RepeatNode(30))
						->addChild(new MoveRelative(-1, 0)))
					->addChild((new BehaviorTree::RepeatNode(30))
						->addChild(new MoveRelative(0, -1)));
		}

		pUnit->brain->execute(pUnit);
	}
}
