#pragma once
#include "common/squad.hpp"
#include <memory>
#include <BWAPI.h>
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/effects/effect.hpp"
#include "search/searchers/searcher.hpp"
#include <exception>
#include <vector>
#include <set>


namespace Bot { namespace Search
{
	class SearchingSquad : public Squad
	{
	public:
		static const int radius = 400;
		shared_ptr<Searcher> searcher;
		
		SearchingSquad(shared_ptr<Searcher> searcher)
			: searcher(searcher)
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
			
			if (!state.isTerminal())
			{
				try {
					int gameframe = BWAPI::Broodwar->getFrameCount();
					vector<shared_ptr<Effect>> actions = (*searcher)(state);

					for (shared_ptr<Effect>& action : actions)
					{
						if (action->isPlayerEffect())
							action->executeOrder(state);
					}
				}
				catch (const std::runtime_error&) {
					throw;
				}
				catch (const std::exception&) {
					throw;
				}
				catch (...) {
					abort();
				}
			}
		}
	};
}}
