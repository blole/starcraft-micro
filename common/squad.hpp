#pragma once
#include "common/common.hpp"

namespace Bot
{
	class Unit;

	class Squad
	{
	public:
		std::list<Unit*> units;

	public:
		virtual ~Squad() {}

	public:
		virtual void addUnit(Unit& unit);
		virtual void removeUnit(Unit& unit);
		virtual void displaySquadLinks();

		virtual void onFrame() = 0;
	};
}
