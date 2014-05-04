#include "behaviors/Flee.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"

using namespace BWAPI;

Flee::Flee()
{
}

void Flee::init(void* agent)
{
	first = true;
	PUnit* pUnit = (PUnit*)agent;
}

BEHAVIOR_STATUS Flee::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	
	// Get all unit in weapon range and flee away from them
	auto enemies = pUnit->unit->getUnitsInWeaponRange(WeaponTypes::Gauss_Rifle,Filter::IsEnemy);
	if(enemies.size()==0)
		return BT_SUCCESS;
	else
	{
		// Compute enemies mean position
		Position meanPosEnemies(0,0);
		for(auto i=enemies.begin();i!=enemies.end();i++)
		{
			meanPosEnemies += (*i)->getPosition();
		}
		meanPosEnemies /= enemies.size();

		// Run away from that position
		Position goal(3*(pUnit->getPosition()) - meanPosEnemies);
		pUnit->unit->move(goal);
		Broodwar->drawLineMap(pUnit->getPosition(),goal,Color(255,0,0));
		return BT_RUNNING;
	}
}
