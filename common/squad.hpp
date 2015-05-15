#pragma once
#include <BWAPI.h>
#include <list>

namespace Bot
{
	class PUnit;

	enum Objective
	{
		MOVING,
		ATTACKING,
		HOLDING
	};

	class Squad
	{
	public:
		std::list<PUnit*> units;

	public:
		virtual void onFrame() = 0;
	
		virtual void addUnit(PUnit* unit);
		virtual void removeUnit(PUnit* unit);
		virtual void displaySquadLinks();

		//virtual void attack(BWAPI::PositionOrUnit positionOrUnit) = 0;
		//virtual void moveTo(BWAPI::Position position) = 0;
	
		Objective getCurrentObjective();
	};
}
