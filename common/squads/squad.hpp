#pragma once
#include "common/units/unit.hpp"

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
			for (auto i = units_.begin(); i != units_.end();)
			{
				if (!(*i)->bwapiUnit->exists())
				{
					Unit& unit = *(*i);
					i = units_.erase(i);
					remove(unit);
				}
				else
					++i;
			}

			for (auto& unit : units())
				unit->update();
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
