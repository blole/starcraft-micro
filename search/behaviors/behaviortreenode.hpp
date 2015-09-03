#pragma once
#include "search/effects/effect.hpp"

namespace Bot { namespace Search
{
	const shared_ptr<Effect> success = std::make_shared<NoEffect>();
	const shared_ptr<Effect> running = std::make_shared<NoEffect>();
	const shared_ptr<Effect> failure = std::make_shared<NoEffect>();

	struct BehaviorTreeNode
	{
	private:
		bool first;
		
	public:
		BehaviorTreeNode()
			: first(true)
		{}
		virtual ~BehaviorTreeNode()
		{}

	public:
		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit)
		{
			shared_ptr<Effect> status;

			if (first)
			{
				first = false;
				status = firstExecute(state, unit);
			}
			else
				status = subsequentExecute(state, unit);

			if (status == success || status == failure)
				first = true;

			return status;
		}

	protected:
		virtual shared_ptr<Effect> firstExecute(GameState& state, Unit& unit) = 0;
		virtual shared_ptr<Effect> subsequentExecute(GameState& state, Unit& unit) = 0;
	};
}}
