#include "flee.hpp"
#include "common/gameunit.hpp"
#include "common/punit.hpp"

using namespace BWAPI;
using namespace Bot;

void Flee::init(void* agent)
{
	first = true;
	PUnit* pUnit = (PUnit*)agent;
	lookForNewPos = true;
}

BEHAVIOR_STATUS Flee::execute(void* agent)
{
	PUnit* pUnit = (PUnit*)agent;
	
	auto type = pUnit->unit->getType();
	auto matrixInfluence = (InfluenceMap::matrixInfluence.find(type))->second;
	int iUnit = pUnit->getPosition().x/32;
	int jUnit = pUnit->getPosition().y/32;
	if(matrixInfluence[iUnit][jUnit] == 0)
		return BT_SUCCESS;
	else
	{
		// Check if bestPos is still safe
		int iBest = bestPos.x/32;
		int jBest = bestPos.y/32;
		if(lookForNewPos || !matrixInfluence[iBest][jBest] ==0)
		{
			// go to the closest safe area
			double bestDist = 10000;
			for(int i=0;i<InfluenceMap::mapWidth;i++)
			{
				for(int j=0;j<InfluenceMap::mapHeight;j++)
				{
					if(matrixInfluence[i][j] == 0)
					{
						Position cellPos(i*32,j*32);
						double dist = cellPos.getApproxDistance(pUnit->getPosition());
						if(abs(dist-InfluenceMap::mapAttackRange[type]) < bestDist)
						{
							bestDist = abs(dist-InfluenceMap::mapAttackRange[type]);
							bestPos = cellPos;
						}
					}
				}
			}
			lookForNewPos = false;
		}
		Broodwar->drawLineMap(pUnit->getPosition(),bestPos,Color(0,0,255));
		pUnit->unit->move(bestPos);
		return BT_RUNNING;
	}
}
