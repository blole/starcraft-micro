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
	virtual float value(Bot::Search::GameState* pGameState);
};

class HitPointDifference : public Heuristic
{
public:
	virtual float value(Bot::Search::GameState* pGameState);
};

class LifeTimeDamage1 : public Heuristic
{
public:
	virtual float value(Bot::Search::GameState* pGameState);
};

class LifeTimeDamage2 : public Heuristic
{
public:
	virtual float value(Bot::Search::GameState* pGameState);
};