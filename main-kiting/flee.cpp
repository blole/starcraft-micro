#include "flee.hpp"
#include "common/unit.hpp"
#include "main-kiting/influencemap.hpp"

using namespace Bot;
using namespace BehaviorTree;

void Flee::init(void* agent)
{
	lookForNewPos = true;
}

BEHAVIOR_STATUS Flee::execute(void* agent)
{
	BWAPI::Unit unit = ((Unit*)agent)->bwapiUnit;

	auto type = unit->getType();
	auto matrixInfluence = (InfluenceMap::matrixInfluence.find(type))->second;
	int iUnit = unit->getPosition().x/32;
	int jUnit = unit->getPosition().y/32;
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
						BWAPI::Position cellPos(i*32,j*32);
						double dist = cellPos.getApproxDistance(unit->getPosition());
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
		Broodwar->drawLineMap(unit->getPosition(),bestPos, BWAPI::Color(0,0,255));
		unit->move(bestPos);
		return BT_RUNNING;
	}
}
