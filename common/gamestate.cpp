#include "common/gamestate.hpp"
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

using namespace Bot;

GameState::GameStateUnitContainer::GameStateUnitContainer(const vector<const Unit*>& playerUnits_, const vector<const Unit*>& enemyUnits_)
{
	for (auto& unit : playerUnits_)
	{
		Unit* clone = unit->clone();
		units.emplace_back(clone);
		playerUnits.emplace_back(clone);
		unitMap.emplace(clone->bwapiUnit, clone);
	}
	for (auto& unit : enemyUnits_)
	{
		Unit* clone = unit->clone();
		units.emplace_back(clone);
		enemyUnits.emplace_back(clone);
		unitMap.emplace(clone->bwapiUnit, clone);
	}
}

// private constructor
GameState::GameState(GameStateUnitContainer uc, unsigned int frame_)
	: frame_(frame_)
	, unitMap(uc.unitMap)
	, units(std::move(uc.units))
	, playerUnits(uc.playerUnits)
	, enemyUnits(uc.enemyUnits)
{
}

// public constructors
GameState::GameState(const vector<const Unit*>& playerUnits, const vector<const Unit*>& enemyUnits)
	: GameState(GameStateUnitContainer(playerUnits, enemyUnits), 0)
{
	for (auto& unit : units)
		unit->firstFrameInitToAddAlreadyActiveEffects(*this);
}
GameState::GameState(const GameState& o)
	: GameState(GameStateUnitContainer(
		reinterpret_cast<const vector<const Unit*>&>(o.playerUnits),
		reinterpret_cast<const vector<const Unit*>&>(o.enemyUnits)),
		o.frame_)
{
	pendingEffects = o.pendingEffects;
}

void GameState::advanceFrames(unsigned int framesToAdvance)
{
	for (unsigned int i=0; i<framesToAdvance; i++)
	{
		frame_++;
		if (!pendingEffects.empty())
		{
			vector<shared_ptr<Effect>> effects = pendingEffects.front();
			pendingEffects.pop_front();

			for (auto& effect : effects)
				effect->applyTo(*this);
		}
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
