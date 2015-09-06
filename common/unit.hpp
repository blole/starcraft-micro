#pragma once
#include "common/common.hpp"
#include <lib/libbehavior/BehaviorTree.h>

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
		BehaviorTree::BehaviorTreeNode* brain;
		Squad* squad;

	private:
		Unit(BWAPI::Unit bwapiUnit)
			: bwapiUnit(bwapiUnit)
			, brain(nullptr)
			, squad(nullptr)
		{}
	};
}
