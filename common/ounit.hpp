#pragma once
#include <BWAPI.h>
#include "common/gameunit.hpp"

namespace Bot
{
	class OUnit : public GameUnit
	{
	public:
		float dammageAttributed;
		int numberOfAttackers;
	
	public:
		OUnit(BWAPI::Unit unit);
	
	public:
		bool willDie();

	public:
		static std::unordered_map<int, OUnit*> units;
	public:
		static OUnit* get(BWAPI::Unit unit);
	
	};
}
