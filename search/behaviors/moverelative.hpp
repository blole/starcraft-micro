#pragma once
#include "search/behaviors/behaviortree.hpp"
#include "search/effects/move.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	struct MoveRelative : BehaviorTreeNode_CRTP<MoveRelative>
	{
	private:
		const BWAPI::Position offset;
		BWAPI::Position origin;

	public:
		MoveRelative(const BWAPI::Position offset)
			: offset(offset)
		{}
		MoveRelative(int x, int y)
			: MoveRelative(BWAPI::Position(x, y))
		{}

		virtual void init(GameState& state, Unit& unit) override
		{
			origin = unit.pos;
		}

		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			if (unit.pos == origin + offset)
				return success;
			else if (!unit.isMoving)
				return make_shared<Move<>>(MoveData(unit.id, offset));
			else
				return running;
		}
	};
}}}
