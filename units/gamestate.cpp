#include "units/gamestate.hpp"
#include "units/unit.hpp"

using namespace Bot::Units;

GameState::GameState(GameState* parent, Action* action)
	: parent(parent)
	, frame(parent->frame)
{}

GameState::GameState(std::vector<BWAPI::Unit> bwapiUnits)
	: parent(nullptr)
{
	if (!GameState::bwapiUnits.empty())
		throw std::runtime_error("previous 'mother' GameState was not destroyed before creating a new one");

	GameState::bwapiUnits = bwapiUnits;
	id_t id = 0;
	for each (BWAPI::Unit bwapiUnit in bwapiUnits)
	{
		units.push_back(Unit::create(this, bwapiUnit, id));
		id++;
	}
}

GameState::~GameState()
{
	GameState::bwapiUnits.clear();
}

std::list<Unit*> GameState::unitsInRange(BWAPI::Position pos, int maxRange)
{
	std::list<Unit*> inRange;

	for each (Unit* unit in units)
	{
		if (!unit->isAlive())
			continue;
		double distance = unit->pos.getDistance(pos);
		if (distance <= maxRange)
			inRange.push_back(unit);
	}

	return inRange;
}

std::list<Unit*> GameState::unitsInRange(BWAPI::Position pos, int minRange, int maxRange)
{
	std::list<Unit*> inRange;

	for each (Unit* unit in units)
	{
		if (!unit->isAlive())
			continue;
		double distance = unit->pos.getDistance(pos);
		if (minRange <= distance && distance <= maxRange)
			inRange.push_back(unit);
	}

	return inRange;
}

std::list<Action*> GameState::possibleActions()
{
	if (!actions.empty())
		return actions;

	if (Settings::branchOnUnit)
	{
		for each (Unit* unit in units)
		{
			if (unit->isAlive())
			{
				actions = unit->possibleActions(this);
				if (!actions.empty())
					return actions;
			}
		}
	}

	throw std::runtime_error("GameState has no possible actions!?!");
}

const Unit* GameState::getUnit(id_t id)
{
	return units[id];
}

Unit* GameState::getUnitModifiable(id_t id)
{
	units[id] = units[id]->clone();
	return units[id];
}

BWAPI::Unit GameState::getBwapiUnit(Unit* unit)
{
	return bwapiUnits[unit->id];
}

void GameState::addEffect(int frameOffset, Action* action)
{
	//TODO
}

float GameState::defaultPolicy()
{
	throw std::runtime_error("not yet implemented");
}

GameState* GameState::takeAction(Action* action)
{
	auto iter = children.find(action);
	if (iter == children.end())
	{
		GameState* child = new GameState(this, action);
		children[action] = child;
		return child;
	}
	else
		return iter->second;
}
