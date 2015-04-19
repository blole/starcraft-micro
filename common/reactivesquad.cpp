#include "common/reactivesquad.hpp"
#include "behaviors/moverelative.hpp"
#include "common/punit.hpp"
#include <exception>

using namespace BWAPI;

ReactiveSquad::ReactiveSquad(std::function<BehaviorTreeNode*()> unitBrain)
	: unitBrain(unitBrain)
{
}

void ReactiveSquad::addUnit(PUnit* unit)
{
	Squad::addUnit(unit);

	delete unit->brain;
	unit->brain = unitBrain();
}

void ReactiveSquad::removeUnit(PUnit* unit)
{
	Squad::removeUnit(unit);

	delete unit->brain;
	unit->brain = nullptr;
}

void ReactiveSquad::onFrame()
{
	units.remove_if([](PUnit* unit){ return !unit->exists(); });

	for (PUnit* unit : units)
		unit->brain->execute(unit);
}
