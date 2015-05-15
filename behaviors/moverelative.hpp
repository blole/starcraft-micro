#pragma once
#include "common/BehaviorTreeNodeFirstSubsequent.hpp"
#include "common/punit.hpp"
#include <BWAPI.h>

namespace Bot { namespace Behaviors
{
	class MoveRelative : public BehaviorTree::BehaviorTreeNodeFirstSubsequent<PUnit>
	{
	public:
		MoveRelative(int x, int y);

		virtual BehaviorTree::BEHAVIOR_STATUS firstExecute(PUnit* unit) override;
		virtual BehaviorTree::BEHAVIOR_STATUS subsequentExecute(PUnit* unit) override;

	private:
		BWAPI::Position origin;
		BWAPI::Position offset;
	};
}}
