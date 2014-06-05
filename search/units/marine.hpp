#pragma once
#include "search/units/unit.hpp"
#include "search/gamestate.hpp"
#include "search/actions/action.hpp"
#include "search/actions/move.hpp"
#include "search/actions/attack.hpp"

namespace Bot { namespace Search
{
	class Terran_Marine : public Unit
	{
	public:
		Terran_Marine(GameState* state, BWAPI::Unit bwapiUnit, id_t id)
			: Unit(state, bwapiUnit, id)
		{

		}

		virtual std::list<Action*> possibleActions(const GameState* state) const
		{
			static const int range = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange();

			std::list<Action*> actions;

			if (!this->groundWeaponCooldown)
			{
				for each (const Unit* unit in state->enemyUnitsInRange(pos, range))
				{
					actions.push_back(new Attack<6>(state, this, unit));
				}
			}

			if (!this->isAttackFrame && !this->isMoving)
			{
				//TODO: precalc all of these directional offsets..
				for (float dir = 0; dir < 3.14f * 2; dir += 3.15f / 4)
					actions.push_back(new Move(state, this, dir));
			}

			return actions;
		}

		virtual Unit* clone() const
		{
			return new Terran_Marine(*this);
		}
	};
}}