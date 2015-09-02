#pragma once
#include "common/common.hpp"
#include "common/squad.hpp"
#include "common/punit.hpp"
#include "search/gamestate.hpp"
#include "search/effects/effect.hpp"
#include "search/searchers/searcher.hpp"


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
