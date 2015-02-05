#include "common/gameunit.hpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

GameUnit::GameUnit(BWAPI::Unit unit)
	: unit(unit)
{
}

int GameUnit::getHp()
{
	return unit->getHitPoints();
}
float GameUnit::getDps()
{
	return 10.0;
}

BWAPI::Position GameUnit::getPosition()
{
	return unit->getPosition();
}

bool GameUnit::exists()
{
	return unit->exists();
}

int GameUnit::getID()
{
	return unit->getID();
}
