#pragma once
#include "common/unit.hpp"

namespace Bot
{
	struct Squad
	{
	private:
		list<Unit*> units_;

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

		const list<Unit*>& units() const
		{
			return units_;
		}
		virtual void add(Unit& unit)
		{
			assert(unit.squad == nullptr);
			unit.squad = this;
			units_.push_back(&unit);
		}
		virtual void remove(Unit& unit)
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
