#pragma once
#include "search/actions/effect.hpp"
#include <cassert>
#include <vector>

namespace Bot { namespace Search
{
	class GameState;

	class CompositeEffect : public Effect
	{
	protected:
		std::vector<Effect*> actions;
	public:

		CompositeEffect(std::vector<Effect*> actions)
			: actions(actions)
		{
			assert(!actions.empty());
		}

		virtual void applyTo(GameState* state) const override
		{
			for (Effect* action : actions)
				action->applyTo(state);
		}

		virtual void executeOrder(GameState* state) const override
		{
			for (Effect* action : actions)
				action->executeOrder(state);
		}
	};
}}
