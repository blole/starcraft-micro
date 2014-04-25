#include "GameUnit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

	// Constructor
GameUnit::GameUnit(BWAPI::Unit unit)
{
	this->unit = unit;
	this->name = unit->getType().c_str();
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