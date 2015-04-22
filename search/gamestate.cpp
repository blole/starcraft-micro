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
	for (unsigned int i = 1; i<=framesToAdvance; i++)
	{
		frame++;
		auto effects = pendingEffects.getEffects(i);
		pendingEffects.advanceFrames(1);
		for (auto effect : effects)
			effect->applyTo(this);
	}
}


//template <template<int, class> class Eff, int A, class B, typename std::enable_if<std::is_base_of<Effect<A, B>, Eff<A, B>>::value>::type>
//void GameState::addEffect(int frameOffset, Eff<A, B>* effect)
//{
	//if (frameOffset == 0)
	//	effect->applyTo(this);
	//else
	//	pendingEffects.addEffect(frameOffset, effect);
//}

void GameState::addEffect(int frameOffset, Action* action)
{
	if (frameOffset == 0)
		action->applyTo(this);
	else
		pendingEffects.addEffect(frameOffset, action);
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
