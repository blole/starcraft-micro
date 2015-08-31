#pragma once
#include "common/common.hpp"
#include "search/behaviors/behaviortreenode.hpp"
#include "search/effects/attack.hpp"

namespace Bot { namespace Search { namespace Behaviors
{
	class AttackClosest : public BehaviorTreeNode
	{
	public:
		virtual shared_ptr<Effect> firstExecute(GameState& state, Unit& unit) override
		{
			if (!unit.groundWeaponCooldown) //TODO: wait for previous attack to finish?
				return running;
			else
			{
				Unit& target = **std::min_element(state.enemyUnits().begin(), state.enemyUnits().end(),
					std::function<double(const unique_ptr<Unit>&)>(
					                 [&](const unique_ptr<Unit>& enemy)
				{
					return (unit.pos - enemy->pos).getLength();
				}));

				return std::make_shared<BeginAttack<>>(TwoUnitEffectData(unit.id, target.id));
			}
		}

		virtual shared_ptr<Effect> subsequentExecute(GameState& state, Unit& unit) override
		{
			if (!unit.isAttackFrame)
				return success;
			else
				return running;
		}
	};
}}}
