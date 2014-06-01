#pragma once
#include <BWAPI.h>
#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include <vector>
#include <list>
#include <map>

class Heuristic
{
public:
	Heuristic();
	virtual float value(Bot::Units::GameState* pGameState);
};

class HitPointDifference : public Heuristic
{
public:
	HitPointDifference();
	virtual float value(Bot::Units::GameState* pGameState);
};

class LifeTimeDamage1 : public Heuristic
{
public:
	LifeTimeDamage1();
	virtual float value(Bot::Units::GameState* pGameState);
};

class LifeTimeDamage2 : public Heuristic
{
public:
	LifeTimeDamage2();
	virtual float value(Bot::Units::GameState* pGameState);
};