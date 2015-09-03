#pragma once
#include "common/common.hpp"
#include "common/squad.hpp"
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/effects/effect.hpp"
#include "search/players/player.hpp"


namespace Bot { namespace Search
{
	template <class SearcherType>
	class SearchingSquad : public Squad
	{
	public:
		static const int radius = 400;
		SearcherType search;
		
		SearchingSquad(const SearcherType& searcher = SearcherType())
			: search(searcher)
		{}
		
		virtual void onFrame() override
		{
			units.remove_if([](PUnit* unit){ return !unit->exists(); });
			
			vector<BWAPI::Unit> playerUnits;
			vector<BWAPI::Unit> enemyUnits;
			
			for (PUnit* unit : units)
			{
				playerUnits.push_back(unit->unit);
				for (auto& u : unit->unit->getUnitsInRadius(radius, BWAPI::Filter::IsEnemy))
				{
					if (std::find(enemyUnits.begin(), enemyUnits.end(), u) == enemyUnits.end())
						enemyUnits.push_back(u);
				}
			}
			
			GameState state(playerUnits, enemyUnits);

			for (auto& unit : state.playerUnits())
			{
				//Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,15), "isAttackFrame: %d", unit->isAttackFrame);
				//Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 0), "isAttackFrame: %d", unit->bwapiUnit->isAttackFrame());

				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0,  0), "   starting: %d", unit->bwapiUnit->isStartingAttack());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 15), "attackFrame: %d", unit->bwapiUnit->isAttackFrame());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 30), "  attacking: %d", unit->bwapiUnit->isAttacking());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 45), "     target: %d", unit->bwapiUnit->getOrderTarget());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 60), "   accelera: %d", unit->bwapiUnit->isAccelerating());
				Broodwar->drawTextMap(unit->pos - BWAPI::Position(0, 75), "   cooldown: %d", unit->bwapiUnit->getGroundWeaponCooldown());

			}

			try
			{
				vector<shared_ptr<Effect>> actions = search(state);

				for (shared_ptr<Effect>& action : actions)
				{
					if (action->isPlayerEffect(state))
						action->executeOrder(state);
				}
			}
			catch (const std::runtime_error&)	{ throw; }
			catch (const std::exception&)		{ throw; }
			catch (...)							{ throw; }
		}
	};
}}
