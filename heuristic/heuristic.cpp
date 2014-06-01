#include "heuristic.hpp"

using namespace Bot::Search;

Heuristic::Heuristic()
{}

HitPointDifference::HitPointDifference() : Heuristic()
{}

float HitPointDifference::value(GameState* pGameState)
{
	auto playerUnits = pGameState->getPlayerUnits();
	auto opponentUnits = pGameState->getOpponentUnits();
	float value = 0;
	foreach(auto unit in playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
		value += currentUnit->getHitPoints();
	}
	foreach(auto unit in opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
		value -	= currentUnit->getHitPoints();
	}
	return value;
}

LifeTimeDamage1::LifeTimeDamage1() : Heuristic()
{}

float LifeTimeDamage1::value(GameState* pGameState)
{
	float value = 0;
	auto playerUnits = pGameState->getPlayerUnits();
	auto opponentUnits = pGameState->getOpponentUnits();
	foreach(auto unit in playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
			
		BWAPI::WeaponType weapon = currentUnit->groundWeapon();
		float damage = weapon->damageAmount();
		float cooldown = weapon->damageCooldown();
		float dpf = damage/cooldown;

		value += dpf * currentUnit->getHitPoints();
	}
	foreach(auto unit in opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
			
		BWAPI::WeaponType weapon = currentUnit->groundWeapon();
		float damage = weapon->damageAmount();
		float cooldown = weapon->damageCooldown();
		float dpf = damage/cooldown;

		value -= dpf * currentUnit->getHitPoints();
	}
	return value;
}

LifeTimeDamage2::LifeTimeDamage2() : Heuristic()
{}

float LifeTimeDamage2::value(GameState* pGameState)
{
	auto playerUnits = pGameState->getPlayerUnits();
	auto opponentUnits = pGameState->getOpponentUnits();
	float value = 0;
	foreach(auto unit in playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
			
		BWAPI::WeaponType weapon = currentUnit->groundWeapon();
		float damage = weapon->damageAmount();
		float cooldown = weapon->damageCooldown();
		float dpf = damage/cooldown;

		value += dpf * std::sqrt(currentUnit->getHitPoints());
	}
	foreach(auto unit in opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(*unit);
			
		BWAPI::WeaponType weapon = currentUnit->groundWeapon();
		float damage = weapon->damageAmount();
		float cooldown = weapon->damageCooldown();
		float dpf = damage/cooldown;

		value -= dpf * std::sqrt(currentUnit->getHitPoints());
	}
	return value;
}

