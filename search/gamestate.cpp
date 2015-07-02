#include "search/gamestate.hpp"
#include "search/unit.hpp"
#include "search/effect.hpp"

using namespace Bot::Search;

std::vector<BWAPI::Unit> GameState::bwapiUnits;
int GameState::playerUnitCount = -1;

GameState::GameState(std::vector<BWAPI::Unit> playerUnits, std::vector<BWAPI::Unit> enemyUnits)
	: frame(0)
{
	GameState::bwapiUnits.clear();
	GameState::playerUnitCount = playerUnits.size();
	std::copy(playerUnits.begin(), playerUnits.end(), std::back_inserter(bwapiUnits));
	std::copy(enemyUnits.begin(), enemyUnits.end(), std::back_inserter(bwapiUnits));

	id_t id = 0;

	for (BWAPI::Unit playerUnit : playerUnits)
		units.push_back(Unit::create(this, playerUnit, id++));

	for (BWAPI::Unit enemyUnit : enemyUnits)
		units.push_back(Unit::create(this, enemyUnit, id++));
}

GameState::~GameState()
{
}

std::list<const Unit*> GameState::unitsInRange(const std::list<const Unit*>& outOf, BWAPI::Position origin, int minRange, int maxRange) const
{
	std::list<const Unit*> inRange;

	for (const Unit* unit : outOf)
	{
		if (!unit->isAlive())
			continue;
		double distance = origin.getDistance(unit->pos);
		if (minRange <= distance && distance <= maxRange)
			inRange.push_back(unit);
	}

	return inRange;
}

std::list<const Unit*> GameState::playerUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const
{
	return unitsInRange(playerUnits(), origin, minRange, maxRange);
}

std::list<const Unit*> GameState::playerUnitsInRange(BWAPI::Position origin, int maxRange) const
{
	return playerUnitsInRange(origin, 0, maxRange);
}

std::list<const Unit*> GameState::enemyUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const
{
	return unitsInRange(enemyUnits(), origin, minRange, maxRange);
}

std::list<const Unit*> GameState::enemyUnitsInRange(BWAPI::Position origin, int maxRange) const
{
	return enemyUnitsInRange(origin, 0, maxRange);
}

const Unit* GameState::getUnit(const id_t id) const
{
	return units[id];
}

Unit* GameState::getUnitModifiable(const id_t id)
{
	units[id] = units[id]->clone();
	return units[id];
}

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i=0; i<framesToAdvance; i++)
	{
		frame++;
		std::vector<Effect*>& effects = pendingEffects.front();
		for (Effect* effect : effects)
			effect->applyTo(this);
		pendingEffects.pop_front();
	}
}

void GameState::queueEffect(unsigned int frameOffset, Effect* effect)
{
	if (frameOffset == 0)
		effect->applyTo(this);
	else
	{
		if (pendingEffects.size() < frameOffset)
			pendingEffects.resize(frameOffset);
		pendingEffects[frameOffset-1].push_back(effect);
	}
}

bool GameState::isTerminal()
{
	bool anyFriendlyAlive = false;
	for (const Unit* unit : playerUnits())
	{
		if (unit->isAlive())
		{
			anyFriendlyAlive = true;
			break;
		}
	}

	bool anyEnemyAlive = false;
	for (const Unit* unit : enemyUnits())
	{
		if (unit->isAlive())
		{
			anyEnemyAlive = true;
			break;
		}
	}

	return !(anyFriendlyAlive && anyEnemyAlive);
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
