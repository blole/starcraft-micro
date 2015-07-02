#pragma once
#include "common/squad.hpp"
#include <memory>
#include <BWAPI.h>
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/effect.hpp"
#include "search/searcher.hpp"
#include <exception>
#include <vector>
#include <set>


namespace Bot { namespace Search
{
	class SearchingSquad : public Squad
	{
	public:
		static const int radius = 400;
		std::shared_ptr<Searcher> searcher;
		
		SearchingSquad(std::shared_ptr<Searcher> searcher)
			: searcher(searcher)
		{}
		
		virtual void onFrame() override
		{
			units.remove_if([](PUnit* unit){ return !unit->exists(); });
			
			std::vector<BWAPI::Unit> playerUnits;
			std::vector<BWAPI::Unit> enemyUnits;
			
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
					std::vector<Effect*> actions = searcher->search(&state);

					for (Effect* action : actions)
					{
						if (action->isPlayerEffect())
							action->executeOrder(&state);
						delete action;
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
