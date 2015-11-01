#pragma once
#include "common/behaviortree/behaviortree.hpp"

namespace Bot { namespace Behaviors
{
	template <bool NonZero>
	struct WaitForAttackFrame : BehaviorTreeNode
	{
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			if (bool(unit.moveCooldown) == NonZero)
				return success;
			else
				return running;
		}
	};
}}
