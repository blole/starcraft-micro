#include "heuristic.hpp"

using namespace Bot::Search;

float HitPointDifference::value(GameState* pGameState)
{
	auto playerUnits = pGameState->playerUnits();
	auto opponentUnits = pGameState->enemyUnits();
	float value = 0;
	for (auto unit : playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
		value += currentUnit->getHitPoints();
	}
	for (auto unit : opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
		value -= currentUnit->getHitPoints();
	}
	return value;
}

float LifeTimeDamage1::value(GameState* pGameState)
{
	float value = 0;
	auto playerUnits = pGameState->playerUnits();
	auto opponentUnits = pGameState->enemyUnits();
	for (auto unit : playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
			
		BWAPI::WeaponType weapon = currentUnit->getType().groundWeapon();
		float damage = (float)weapon.damageAmount();
		float cooldown = (float)weapon.damageCooldown();
		float dpf = damage/cooldown;

		value += dpf * currentUnit->getHitPoints();
	}
	for (auto unit : opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
			
		BWAPI::WeaponType weapon = currentUnit->getType().groundWeapon();
		float damage = (float)weapon.damageAmount();
		float cooldown = (float)weapon.damageCooldown();
		float dpf = damage/cooldown;

		value -= dpf * currentUnit->getHitPoints();
	}
	return value;
}

float LifeTimeDamage2::value(GameState* pGameState)
{
	auto playerUnits = pGameState->playerUnits();
	auto opponentUnits = pGameState->enemyUnits();
	float value = 0;
	for (auto unit : playerUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
			
		BWAPI::WeaponType weapon = currentUnit->getType().groundWeapon();
		float damage = (float)weapon.damageAmount();
		float cooldown = (float)weapon.damageCooldown();
		float dpf = damage/cooldown;

		value += dpf * std::sqrt((float)currentUnit->getHitPoints());
	}
	for (auto unit : opponentUnits)
	{
		BWAPI::Unit currentUnit = pGameState->getBwapiUnit(unit->id);
			
		BWAPI::WeaponType weapon = currentUnit->getType().groundWeapon();
		float damage = (float)weapon.damageAmount();
		float cooldown = (float)weapon.damageCooldown();
		float dpf = damage/cooldown;

		value -= dpf * std::sqrt((float)currentUnit->getHitPoints());
	}
	return value;
}

