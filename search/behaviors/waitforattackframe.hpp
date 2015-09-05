#pragma once
#include "search/behaviors/behaviortree.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	template <bool value>
	struct WaitForAttackFrame : BehaviorTreeNode
	{
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			if (unit.isAttackFrame == value)
				return success;
			else
				return running;
		}
	};
}}}
