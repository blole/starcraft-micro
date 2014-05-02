#include "behaviors/MoveSquadToPosition.hpp"
#include "common/GameUnit.hpp"
#include "common/PUnit.hpp"
//#include <math.h>


using namespace BWAPI;

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

		float thetaS = atan2((float)squadGoal.y,(float)squadGoal.x);
		float theta = atan2((float)squad->positionToAttack.getPosition().y - squad->positionSquad.y,
					(float)squad->positionToAttack.getPosition().y - squad->positionSquad.x);
		float r0 = squadGoal.getDistance(Position(0,0));
		float step = 2 * 3.14 / (squad->units.size());
		float r = radiusFormation;

 		for(auto i=squad->units.begin(); i!=squad->units.end();i++)
 		{
			// Set the unit goal (using line formation)
			int xUnit = r0 * cos(thetaS) + r * sin(theta);
			int yUnit = r0 * sin(thetaS) - r * cos(theta);
			Broodwar->drawLineMap((*i)->getPosition(),Position(xUnit,yUnit),Color(0,255,255));
			(*i)->unit->move(Position(xUnit,yUnit));
			theta += step;
 		}

 		return BT_RUNNING;
	}
}
