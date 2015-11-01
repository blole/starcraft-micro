#pragma once
#include "common/common.hpp"
#include "common/effects/effect.hpp"

namespace Bot { namespace Effects
{
	class CompositeEffect : public Effect
	{
	protected:
		vector<shared_ptr<Effect>> actions;
	public:

		CompositeEffect(const vector<shared_ptr<Effect>>& actions)
			: actions(actions)
		{
			assert(!actions.empty());
		}

		virtual void applyTo(GameState& state) const override
		{
			for (auto& action : actions)
				action->applyTo(state);
		}

		virtual void executeOrder(GameState& state) const override
		{
			for (auto& action : actions)
				action->executeOrder(state);
		}
	};
}}
