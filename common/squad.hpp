#pragma once
#include "common/common.hpp"

namespace Bot
{
	class PUnit;

	class Squad
	{
	public:
		std::list<PUnit*> units;

	public:
		virtual ~Squad() {}

	public:
		virtual void addUnit(PUnit* unit);
		virtual void removeUnit(PUnit* unit);
		virtual void displaySquadLinks();

		virtual void onFrame() = 0;
	};
}
