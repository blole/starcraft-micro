#pragma once
#include "common/common.hpp"
#include "search/behaviors/behaviortreenode.hpp"
#include "search/gamestate.hpp"
#include "search/units/unit.hpp"
#include "search/effects/move.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	class MoveRelative : public BehaviorTreeNode
	{
	private:
		const float direction;

	public:
		MoveRelative(float direction)
			: direction(direction)
		{}

		virtual shared_ptr<Effect> firstExecute(GameState& state, Unit& unit) override
		{
			return std::make_shared<Move>(unit, direction);
		}

		virtual shared_ptr<Effect> subsequentExecute(GameState& state, Unit& unit) override
		{
			if (!unit.isMoving)
				return success;
			else
				return running;
		}
	};
}}}
