#include "bot/reactivesquad.hpp"
#include "behaviors/moverelative.hpp"
#include "common/PUnit.hpp"
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

	for each(PUnit* unit in units)
		unit->brain->execute(unit);
}
