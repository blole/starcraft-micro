#pragma once
#include "common/gameunit.hpp"
#include "common/punit.hpp"
#include "common/ounit.hpp"
#include <BWAPI.h>
#include <lib/libbehavior/BehaviorTree.h>
#include "squadmanager.hpp"
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
