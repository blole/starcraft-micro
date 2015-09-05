#pragma once
#include "search/behaviors/behaviortree.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	// A node that repeats its child a specified number of times.
	// If the child returns BT_FAILURE, RepeatNode will also return BT_FAILURE.
	// However, if the child returns BT_SUCCESS and it hasn't completed the specified number of repetitions,
	// RepeatNode will restart it and continue returning BT_RUNNING.
	// \param repeats the number of times to repeat. -1 indicates the node should repeat indefinitely.
	template <class BehaviorNodeType, int repeats = -1>
	struct Repeat : BehaviorTreeNode_CRTP<Repeat<BehaviorNodeType, repeats>>
	{
	protected:
		BehaviorNodeType behavior;
		int successes;
		bool doInit;
	public:
		Repeat(BehaviorNodeType behavior = BehaviorNodeType())
			: behavior(behavior)
			, successes(0)
			, doInit(true) //to allow not calling init first
		{}

		virtual void init(GameState& state, Unit& unit) override
		{
			successes = 0;
			doInit = false;
			behavior.init(state, unit);
		}

		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			if (successes == repeats)
				return success;

			if (doInit)
			{
				behavior.init(state, unit);
				doInit = false;
			}

			shared_ptr<Effect> status = behavior.execute(state, unit);
			if (status == success)
			{
				successes++;
				doInit = true;

				if (successes < repeats || repeats < 0)
					return running;
			}
			else
				return status;
		}
	};

	template <class BehaviorNodeType>
	struct Root : Repeat<BehaviorNodeType>
	{
	public:
		Root(BehaviorNodeType behavior = BehaviorNodeType())
			: Repeat(behavior)
		{}

		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			shared_ptr<Effect> status = Repeat<BehaviorNodeType, -1>::execute(state, unit);
			if (status == failure)
			{
				successes = 0;
				doInit = true;
			}
			return status;
		}
	};
}}}
