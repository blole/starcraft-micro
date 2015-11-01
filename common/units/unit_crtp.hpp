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

		virtual void onFrame() override
		{
			Unit::onFrame();

			if (bwapiUnit->isStartingAttack())
			{
				groundWeaponCooldown = bwapiUnit->getGroundWeaponCooldown();
				moveCooldown = 6;
			}
		}

		virtual shared_ptr<Effect> Unit::getActualOrders() override
		{
			if (bwapiUnit->isStartingAttack())
				return make_shared<typename Derived::Attack>(*this, Unit::get(bwapiUnit->getOrderTarget()));

			return make_shared<Effects::NoEffect>();
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
