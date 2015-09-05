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
			static std::unordered_map<int, unique_ptr<Unit>> units;
			int id = bwapiUnit->getID();
			auto iter = units.find(id);
			if (iter == units.end())
				iter = units.emplace(id, unique_ptr<Unit>(new Unit(bwapiUnit))).first;
			return *iter->second;
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
