#pragma once
#include "common/BehaviorTreeNodeFirstSubsequent.hpp"
#include "common/common.hpp"
#include "common/unit.hpp"

namespace Bot { namespace Behaviors
{
	class MoveRelative : public BehaviorTree::BehaviorTreeNodeFirstSubsequent<Unit>
	{
	public:
		MoveRelative(int x, int y);

		virtual BehaviorTree::BEHAVIOR_STATUS firstExecute(Unit* unit) override;
		virtual BehaviorTree::BEHAVIOR_STATUS subsequentExecute(Unit* unit) override;

	private:
		BWAPI::Position origin;
		BWAPI::Position offset;
	};
}}
