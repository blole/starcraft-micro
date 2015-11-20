#pragma once
#include "common/units/unit.hpp"
#include "common/effects/effect.hpp"

namespace Bot
{
	template <typename Derived, const BWAPI::UnitType& UnitType>
	class Unit_CRTP : public Unit
	{
	public:
		Unit_CRTP(BWAPI::Unit bwapiUnit)
			: Unit(bwapiUnit)
		{}

		virtual vector<shared_ptr<Effect>> Unit::getNewEffects() const override
		{
			vector<shared_ptr<Effect>> orders;
			if (bwapiUnit->isStartingAttack())
				orders.push_back(attack(bwapiUnit->getOrderTarget()));
			return orders;
		}


		virtual const BWAPI::UnitType& unitType() override
		{
			return UnitType;
		}

		virtual Unit* clone() const final override
		{
			return new Derived(static_cast<const Derived&>(*this));
		}
	};
}
