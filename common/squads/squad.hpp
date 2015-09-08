#pragma once
#include "common/units/unit.hpp"

namespace Bot
{
	struct Squad
	{
	private:
		list<Search::Unit*> units_;

	public:
		virtual ~Squad() {}

		virtual void onFrame() = 0
		{
			for (auto& unit : units_)
			{
				if (!unit->bwapiUnit->exists())
					remove(*unit);
			}
		}

		const list<Search::Unit*>& units() const
		{
			return units_;
		}
		virtual void add(Search::Unit& unit)
		{
			assert(unit.squad == nullptr);
			unit.squad = this;
			units_.push_back(&unit);
		}
		virtual void remove(Search::Unit& unit)
		{
			assert(unit.squad == this);
			units_.remove(&unit);
			unit.squad = nullptr;
		}
		void displaySquadLinks() const
		{
			for (auto& i : units())
			{
				for (auto& j : units())
					Broodwar->drawLineMap(i->bwapiUnit->getPosition(), j->bwapiUnit->getPosition(), BWAPI::Colors::Green);
			}
		}
	};
}
