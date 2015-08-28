#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/effect.hpp"

using namespace Bot::Search;

vector<unique_ptr<Unit>> createUnits(const GameState& state, const vector<BWAPI::Unit>& playerBwapiUnits, const vector<BWAPI::Unit>& enemyBwapiUnits)
{
	id_t id = 0;
	vector<unique_ptr<Unit>> units;
	units.reserve(playerBwapiUnits.size() + enemyBwapiUnits.size());

	for (BWAPI::Unit playerUnit : playerBwapiUnits)
		units.push_back(Unit::create(state, playerUnit, id++));
	for (BWAPI::Unit enemyUnit : enemyBwapiUnits)
		units.push_back(Unit::create(state, enemyUnit, id++));

	return units;
}

// private constructor
GameState::GameState(unsigned int frame_, unsigned int playerUnitCount, vector<unique_ptr<Unit>> units)
	: frame_(frame_)
	, units(std::move(units))
	, playerUnitCount(playerUnitCount)
{
}

// public constructors
GameState::GameState(const vector<BWAPI::Unit>& playerBwapiUnits, const vector<BWAPI::Unit>& enemyBwapiUnits)
	: GameState(0, playerBwapiUnits.size(), createUnits(*this, playerBwapiUnits, enemyBwapiUnits))
{
	for (auto& unit : units)
		unit->firstFrameInitToAddAlreadyActiveEffects(*this);
}
GameState::GameState(const GameState& other)
	: GameState(other.frame(), other.playerUnitCount, clone(other.units))
{
	pendingEffects = other.pendingEffects;
}

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i=0; i<framesToAdvance; i++)
	{
		frame_++;
		vector<shared_ptr<Effect>> effects = pendingEffects.front();
		pendingEffects.pop_front();

		for (auto& effect : effects)
			effect->applyTo(*this);
	}
}

void GameState::queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect)
{
	if (frameOffset == 0)
		effect->applyTo(*this);
	else
	{
		if (pendingEffects.size() < frameOffset)
			pendingEffects.resize(frameOffset);
		pendingEffects[frameOffset-1].push_back(effect);
	}
}
