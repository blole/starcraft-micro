#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/units/unit.hpp"

using namespace Bot::Search;

std::vector<BWAPI::Unit> GameState::bwapiUnits;

GameState::GameState(const GameState* parent, Action* action)
	: parent(parent)
	, frame(parent->frame)
	, units(parent->units)
	, pendingEffects(parent->pendingEffects)
{
	action->applyTo(this);
}

GameState::GameState(const GameState* parent)
	: parent(parent)
	, frame(parent->frame)
	, units(parent->units)
	, pendingEffects(parent->pendingEffects)
{
	//TODO: step through pendingEffects one frame at a time until possibleActions() returns something
	//frame++;
}

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

std::list<Unit*> GameState::unitsInRange(BWAPI::Position pos, int maxRange) const
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

std::list<Unit*> GameState::unitsInRange(BWAPI::Position pos, int minRange, int maxRange) const
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

void GameState::setPossibleActions(std::list<Action*> possibleActions)
{
	this->possibleActions = possibleActions;
	//maybe clear/setup expandedChildren here?
}

const Unit* GameState::getUnit(id_t id) const
{
	return units[id];
}

Unit* GameState::getUnitModifiable(id_t id)
{
	units[id] = units[id]->clone();
	return units[id];
}

BWAPI::Unit GameState::getBwapiUnit(id_t id) const
{
	return bwapiUnits[id];
}

BWAPI::Unit GameState::getBwapiUnit(const Unit* unit) const
{
	return bwapiUnits[unit->id];
}

void GameState::addEffect(int frameOffset, Action* action)
{
	pendingEffects.addEffect(frameOffset, action);
}

float GameState::defaultPolicy()
{
	throw std::runtime_error("not yet implemented");
}

GameState* GameState::takeAction(Action* action)
{
	auto iter = expandedChildren.find(action);
	if (iter == expandedChildren.end())
	{
		GameState* child = new GameState(this, action);
		expandedChildren[action] = child;
		return child;
	}
	else
		return iter->second;
}
