#include "movesquadtoposition.hpp"
#include "common/gameunit.hpp"
#include "common/punit.hpp"
//#include <math.h>


using namespace BWAPI;
using namespace Bot;

MoveSquadToPosition::MoveSquadToPosition()
{
	radiusFormation = 30;
}

void MoveSquadToPosition::init(void* agent)
{
	first = true;
	SquadManager* squad = (SquadManager*)agent;
}

BEHAVIOR_STATUS MoveSquadToPosition::execute(void* agent)
{

	SquadManager* squad = (SquadManager*)agent;
	Broodwar->drawLineMap(squad->positionSquad,squad->positionToAttack.getPosition(),Color(255,0,0));


	if(squad->isInPosition())
		return BT_SUCCESS;
	else
	{
		// Evaluate if squad need to be regroup
		bool needToRegroup = false;
		for(auto i=squad->units.begin(); i!=squad->units.end();i++)
 		{
			if(squad->positionSquad.getApproxDistance((*i)->getPosition()) > 3*radiusFormation)
			{
				needToRegroup = true;
				break;
			}
 		}
		Position squadGoal = needToRegroup ? squad->positionSquad : squad->positionToAttack.getPosition();

		float thetaS = (float)atan2((float)squadGoal.y,(float)squadGoal.x);
		float theta = (float)atan2((float)squad->positionToAttack.getPosition().y - squad->positionSquad.y,
					(float)squad->positionToAttack.getPosition().y - squad->positionSquad.x);
		float r0 = (float)squadGoal.getDistance(Position(0,0));
		float step = (float)2 * 3.14f / ((float)squad->units.size());
		float r = radiusFormation;

 		for(auto i=squad->units.begin(); i!=squad->units.end();i++)
 		{
			// Set the unit goal (using line formation)
			int xUnit = (int)(r0 * cos(thetaS) + r * sin(theta));
			int yUnit = (int)(r0 * sin(thetaS) - r * cos(theta));
			Broodwar->drawLineMap((*i)->getPosition(),Position(xUnit,yUnit),Color(0,255,255));
			(*i)->unit->move(Position(xUnit,yUnit));
			theta += step;
 		}

 		return BT_RUNNING;
	}
}
