#pragma once
#include "common/generals/general.hpp"

namespace Bot { namespace Generals
{
	template <class SquadType, int units_per_squad = 5>
	struct LimitedSquadSize : GeneralWithSquads<SquadType>
	{
		virtual void onStart() override
		{
			// Create squads of 'units_per_squad' units
			for (auto& unit : Broodwar->self()->getUnits())
			{
				if (unit->getType() == BWAPI::UnitTypes::Terran_Marine)
				{
					if (squads.empty() || squads.back().units.size() >= units_per_squad)
						squads.emplace_back();
					squads.back().addUnit(Unit::get(unit));
				}
			}
		}

		virtual void onFrame() override
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
	};
}}
