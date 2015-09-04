#include "squadfire.hpp"
#include "squadmanager.hpp"

using namespace BWAPI;
using namespace Bot;

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
				(*i)->bwapiUnit->move((*i)->getPosition() + (*i)->getPosition() - (*i)->target.getPosition(),true);
			}
		}
		return BT_RUNNING;
	}
}
