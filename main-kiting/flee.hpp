#pragma once
#include "search/behaviortree/behaviortree.hpp"
#include "search/effects/move.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	struct Flee : BehaviorTreeNode_CRTP<Flee>
	{
	private:
		BWAPI::Position bestPos;
		bool lookForNewPos;
	public:
		virtual void init(GameState& state, Unit& unit) override
		{
			lookForNewPos = true;
		}

		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			auto type = unit.bwapiUnit->getType();
			auto matrixInfluence = (InfluenceMap::matrixInfluence.find(type))->second;
			int iUnit = unit.pos.x / 32;
			int jUnit = unit.pos.y / 32;
			if (matrixInfluence[iUnit][jUnit] == 0)
				return success;
			else
			{
				// Check if bestPos is still safe
				int iBest = bestPos.x / 32;
				int jBest = bestPos.y / 32;
				if (lookForNewPos || matrixInfluence[iBest][jBest] != 0)
				{
					// go to the closest safe area
					double bestDist = 10000;
					for (int i = 0;i < InfluenceMap::mapWidth;i++)
					{
						for (int j = 0;j < InfluenceMap::mapHeight;j++)
						{
							if (matrixInfluence[i][j] == 0)
							{
								BWAPI::Position cellPos(i * 32, j * 32);
								double dist = cellPos.getApproxDistance(unit.pos);
								if (abs(dist - InfluenceMap::mapAttackRange[type]) < bestDist)
								{
									bestDist = abs(dist - InfluenceMap::mapAttackRange[type]);
									bestPos = cellPos;
								}
							}
						}
					}
					lookForNewPos = false;
				}
				Broodwar->drawLineMap(unit.pos, bestPos, BWAPI::Colors::Blue);
				
				return make_shared<Effects::Move<>>(unit, bestPos-unit.pos);
			}
		}
	};
}}}
