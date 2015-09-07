#pragma once
#include "common/common.hpp"

namespace Bot
{
	struct Squad;

	class Unit
	{
	public:
		static Unit& get(BWAPI::Unit bwapiUnit)
		{
			static const int key = 87073;
			void* ptr = bwapiUnit->getClientInfo(key);
			if (!ptr)
			{
				ptr = new Unit(bwapiUnit);
				bwapiUnit->setClientInfo(ptr, key);
			}
			return *static_cast<Unit*>(ptr);
		}

	public:
		const BWAPI::Unit bwapiUnit;
		Squad* squad;

	private:
		Unit(BWAPI::Unit bwapiUnit)
			: bwapiUnit(bwapiUnit)
			, squad(nullptr)
		{}
	};
}
