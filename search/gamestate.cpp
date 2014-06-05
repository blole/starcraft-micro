#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/units/unit.hpp"

using namespace Bot::Search;

std::vector<BWAPI::Unit> GameState::bwapiUnits;
int GameState::playerUnitCount = -1;

GameState::GameState(const GameState* parent, Action* action)
	: frame(parent->frame)
	, units(parent->units)
	, pendingEffects(parent->pendingEffects)
{
	action->applyTo(this);
}

GameState::GameState(std::vector<BWAPI::Unit> bwapiUnits)
	: frame(0)
{
	GameState::bwapiUnits = bwapiUnits;
	GameState::playerUnitCount = 0;
	id_t id = 0;
	for each (BWAPI::Unit bwapiUnit in bwapiUnits)
	{
		units.push_back(Unit::create(this, bwapiUnit, id));
		id++;

		//TODO: this assumes enemy units come after player units
		if (bwapiUnit->getPlayer() == BWAPI::Broodwar->self())
			GameState::playerUnitCount++;
	}
}

GameState::~GameState()
{
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

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i = 0; i<framesToAdvance; i++)
	{
		for each (auto effect in pendingEffects.getEffects(i))
			effect->applyTo(this);
		frame++; //TODO do this before or after applying effects?
	}
	pendingEffects.advanceFrames(framesToAdvance);
}

void GameState::enqueueEffect(int frameOffset, Action* action)
{
	pendingEffects.addEffect(frameOffset, action);
}

bool GameState::isTerminal()
{
	bool anyFriendlyAlive = false;
	for each (const Unit* unit in playerUnits())
	{
		if (unit->isAlive())
		{
			anyFriendlyAlive = true;
			break;
		}
	}

	bool anyEnemyAlive = false;
	for each (const Unit* unit in enemyUnits())
	{
		if (unit->isAlive())
		{
			anyEnemyAlive = true;
			break;
		}
	}

	return !anyFriendlyAlive && !anyEnemyAlive;
}

std::list<const Unit*> GameState::playerUnits() const
{
	return std::list<const Unit*>(getUnits().begin(), getUnits().begin() + playerUnitCount);
}

std::list<const Unit*> GameState::enemyUnits() const
{
	return std::list<const Unit*>(getUnits().begin() + playerUnitCount, getUnits().end());
}

const std::vector<const Unit*>& GameState::getUnits() const
{
	return (std::vector<const Unit*>&)units;
}
