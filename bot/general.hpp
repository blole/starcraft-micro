#pragma once
#include <BWAPI.h>
#include <list>
#include "common/squad.hpp"

class General
{
public:
	std::function<Squad*()> newSquad;
	std::list<Squad*> squads;

public:
	General(std::function<Squad*()> newSquad);

	void onStart();
	void onFrame();
};
