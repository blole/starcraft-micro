#pragma once
#include "common/common.hpp"

namespace Bot
{
	class Unit;

	struct Squad
	{
		std::list<Unit*> units;

		virtual ~Squad() {}

		virtual void addUnit(Unit& unit);
		virtual void removeUnit(Unit& unit);
		virtual void displaySquadLinks();

		virtual void onFrame() = 0;
	};
}
