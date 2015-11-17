#pragma once
#include "common/behaviortree/behaviortree.hpp"

namespace Bot { namespace Behaviors
{
	// Executes its children in the order they were added.
	// Returns the first non-success-status of it's children.
	template <typename ...T>
	struct Sequence : BehaviorTreeInternalNode<Sequence<T...>>
	{
	protected:
		unsigned int n;

	public:
		Sequence()
			: n(0)
		{
			[](...) {}((addChild(make_unique<T>()))...);
			//auto list = {(addChild(make_unique<T>()))...};
		}

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
}}
