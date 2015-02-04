#include "common/gameunit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
GameUnit::GameUnit(BWAPI::Unit unit) : unit(unit),name(unit->getType().c_str())
{
}

	// Methods
int GameUnit::getHp()
{
	return this->unit->getHitPoints();
}
float GameUnit::getDps()
{
	return 10.0;
}
BWAPI::Position GameUnit::getPosition()
{
	return this->unit->getPosition();
}

bool GameUnit::exists()
{
	return this->unit->exists();
}

int GameUnit::getID()
{
	return this->unit->getID();
}
