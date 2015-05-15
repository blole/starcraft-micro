#pragma once
#include <lib/libbehavior/BehaviorTree.h>
#include <BWAPI.h>

namespace Bot { namespace Behaviors
{
	class AttackClosest : public BehaviorTree::BehaviorTreeNode
	{
	public:
		virtual void init(void* agent) override;
		virtual BehaviorTree::BEHAVIOR_STATUS execute(void* agent) override;

	private:
		bool first;
		bool hasStartAttack;
	};
}}
