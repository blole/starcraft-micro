#include "SquadFire.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"
#include "SquadManager.hpp"

using namespace BWAPI;

SquadFire::SquadFire()
{
}

void SquadFire::init(void* agent)
{
	first = true;
	SquadManager* squad = (SquadManager*)agent;
}

BEHAVIOR_STATUS SquadFire::execute(void* agent)
{
	SquadManager* squad = (SquadManager*)agent;

	if(!squad->positionToAttack.getUnit()->exists())
		return BT_SUCCESS;
	else
	{
		for(auto i=squad->units.begin(); i!=squad->units.end();i++)
		{
			if((*i)->target.getUnit() != squad->positionToAttack.getUnit())
			{
				(*i)->attackTarget(squad->positionToAttack,true);
				(*i)->unit->move((*i)->getPosition() + (*i)->getPosition() - (*i)->target.getPosition(),true);
			}
		}
		return BT_RUNNING;
	}
}
