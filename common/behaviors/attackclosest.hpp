#pragma once
#include "common/common.hpp"
#include "common/behaviortree/behaviortree.hpp"
#include "common/effects/attack.hpp"

namespace Bot { namespace Behaviors
{
	struct AttackClosest : BehaviorTreeNode_CRTP<AttackClosest>
	{
	private:
		int fsm;
	public:
		virtual void init(GameState& state, Unit& unit) override
		{
			fsm = 0;
		}

		virtual shared_ptr<Effect> execute(GameState& state, Unit& unit) override
		{
			switch (fsm)
			{
			case 0:
				if (state.enemyUnits.empty())
					return failure;
				else
				{
					Unit& target = **std::min_element(state.enemyUnits.begin(), state.enemyUnits.end(),
						std::function<double(const Unit*)>(
							[&unit](const Unit* enemy)
					{
						return (unit.pos - enemy->pos).getLength();
					}));
					fsm++;
					return make_shared<Effects::BeginAttack<>>(Effects::TwoUnitEffectData(unit, target));
				}
			case 1:
				if (!unit.isAttackFrame)
					return running;
				fsm++;
			case 2:
				if (unit.isAttackFrame)
					return running;
				fsm++;
			case 3:
				return success;
			default:
				return failure;
			}
		}
	};
}}
