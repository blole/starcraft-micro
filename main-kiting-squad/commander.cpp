#include "main-kiting-squad/commander.hpp"
#include "main-kiting-squad/squadfire.hpp"
#include "main-kiting-squad/movesquadtoposition.hpp"
#include "common/unit.hpp"
#include <lib/libbehavior/BehaviorTree.h>

using namespace Bot;


void Commander::onStart()
{
	// Create squads of 'nbUnitPerSquad' units
	for (auto& unit : Broodwar->self()->getUnits())
	{
		if (unit->getType() == BWAPI::UnitTypes::Terran_Marine)
		{
			if (squads.empty() || squads.back().units.size() >= nbUnitPerSquad)
				squads.emplace_back();
			squads.back().addUnit(Unit::get(unit));
		}
	}

	// Add a 'brain' to a squad
	for(auto& squad : squads)
	{
		if (squad.brain == nullptr)
			squad.brain = (new ParallelNode())
							->addChild((new RepeatNode(-1))
								->addChild(new BoolCondition<SquadManager>(&SquadManager::isInPosition,true))
								->addChild(new SquadFire()))
							->addChild(new MoveSquadToPosition());
	}
}

void Commander::onFrame()
{
	auto squadIt = squads.begin();
	for (auto& enemyUnit : BWAPI::Broodwar->getAllUnits())
	{
		if (!enemyUnit->exists() || enemyUnit->getPlayer() == Broodwar->self())
			continue;
		else if (squadIt == squads.end())
			break;
		else if (!squadIt->positionToAttack.getUnit()->exists())
			squadIt->positionToAttack = enemyUnit;
		
		++squadIt;
	}

	for (auto& squad : squads)
		squad.brain->execute(&squad);
}
