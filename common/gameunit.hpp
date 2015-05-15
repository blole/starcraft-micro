#pragma once
#include <BWAPI.h>

namespace Bot
{
	class GameUnit
	{
	public:
		BWAPI::Unit unit;
	
	public:
		GameUnit(BWAPI::Unit unit);
	
	public:
		int getHp();
		float getDps();
		BWAPI::Position getPosition();
		bool exists();
		int getID();
	};
}
