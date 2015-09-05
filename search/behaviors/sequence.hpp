#pragma once
#include "search/behaviors/behaviortree.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	// Executes its children in the order they were added.
	// Returns the first non-success-status of it's children.
	struct Sequence : BehaviorTreeInternalNode<Sequence>
	{
	protected:
		unsigned int n;

	public:
		Sequence()
			: n(0)
		{}

		virtual void init(GameState& state, Unit& unit) override
		{
			n = 0;
			for (auto& child : children)
				child->init(state, unit);
		}
		
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			while (n < children.size())
			{
				shared_ptr<Effect> status = children[n]->execute(state, unit);

				if (status == success)
					n++;
				else
					return status;
			}
			return success;
		}
	};
}}}
