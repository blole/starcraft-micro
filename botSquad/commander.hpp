#pragma once
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"
#include "common/OUnit.hpp"
#include <BWAPI.h>
#include <lib/libbehavior/BehaviorTree.h>
#include "SquadManager.hpp"
#include <set>


using namespace BehaviorTree;

class Commander
{
public:
	// Attributes
	std::set<PUnit*> pAllUnits;
	std::set<OUnit*> oAllUnits;
	std::set<SquadManager*> squads;
	// Methods
	void init();
	void update();
	void destroyUnit(BWAPI::Unit unit);

private:
	const static int nbUnitPerSquad = 5;
};
