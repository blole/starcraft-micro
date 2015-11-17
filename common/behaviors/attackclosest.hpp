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
				{
					fsm = -1;
					return failure;
				}
				else
				{
					if (unit.groundWeaponCooldown)
						return running; //TODO: maybe fail here?

					Unit& target = **std::min_element(state.enemyUnits.begin(), state.enemyUnits.end(),
						std::function<double(const Unit*)>(
							[&unit](const Unit* enemy)
					{
						double dist = unit.pos.getDistance(enemy->pos);
						if (dist < unit.groundWeaponMinRange())
							dist += 10000; //don't prioritize the too close ones
						return dist;
					}));

					double dist = unit.pos.getDistance(target.pos);
					if (dist > unit.groundWeaponMaxRange()) //move towards
						return unit.move(target.pos - unit.pos);
					if (unit.groundWeaponMinRange() > dist) //move away
						return unit.move(unit.pos - target.pos);

					fsm++;
					return unit.attack(target);
				}
			case 1:
				if (!unit.moveCooldown)
					return running;
				fsm++;
			case 2:
				if (unit.moveCooldown)
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
