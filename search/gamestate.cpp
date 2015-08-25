#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"

using namespace Bot::Search;

vector<BWAPI::Unit> GameState::bwapiUnits;
int GameState::playerUnitCount = -1;

GameState::GameState(vector<BWAPI::Unit> playerBwapiUnits, vector<BWAPI::Unit> enemyBwapiUnits)
	: frame(0)
{
	GameState::bwapiUnits.clear();
	GameState::playerUnitCount = playerBwapiUnits.size();
	std::copy(playerBwapiUnits.begin(), playerBwapiUnits.end(), std::back_inserter(bwapiUnits));
	std::copy(enemyBwapiUnits.begin(), enemyBwapiUnits.end(), std::back_inserter(bwapiUnits));

	id_t id = 0;

	for (BWAPI::Unit playerUnit : playerBwapiUnits)
		units.push_back(Unit::create(this, playerUnit, id++));

	for (BWAPI::Unit enemyUnit : enemyBwapiUnits)
		units.push_back(Unit::create(this, enemyUnit, id++));
}

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i=0; i<framesToAdvance; i++)
	{
		frame++;
		vector<Effect*> effects = pendingEffects.front();
		pendingEffects.pop_front();

		for (Effect* effect : effects)
			effect->applyTo(this);
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
	for (auto& unit : playerUnits())
	{
		if (unit->isAlive())
		{
			anyFriendlyAlive = true;
			break;
		}
	}

	bool anyEnemyAlive = false;
	for (auto& unit : enemyUnits())
	{
		if (unit->isAlive())
		{
			anyEnemyAlive = true;
			break;
		}
	}

	return !anyFriendlyAlive || !anyEnemyAlive;
}
