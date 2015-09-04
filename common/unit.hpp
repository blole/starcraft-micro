#pragma once
#include "common/common.hpp"
#include <lib/libbehavior/BehaviorTree.h>

namespace Bot
{
	class Squad;

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
		BWAPI::PositionOrUnit target;
		Squad* squad;
		bool hasTarget;

	private:
		Unit(BWAPI::Unit bwapiUnit)
			: bwapiUnit(bwapiUnit)
			, brain(nullptr)
			, target(nullptr)
			, squad(nullptr)
			, hasTarget(false)
		{}

	public:
		int getHp();
		float getDps();
		BWAPI::Position getPosition();
		bool exists();

		bool isAttacking();
		void setTarget(BWAPI::PositionOrUnit newTarget);
		void attackTarget(BWAPI::PositionOrUnit newTarget, bool addToQueue);
	};
}
