#pragma once
#include "common/common.hpp"

namespace Bot
{
	struct Squad;
	class Effect;
	class GameState;

	class Unit
	{
	private:
		int hp_;

	public:
		const int id;
		const BWAPI::Unit bwapiUnit;
		const bool isPlayer;
		
		Squad* squad;
		BWAPI::Position pos;
		bool isMoving;
		int moveCooldown;
		int groundWeaponCooldown;

	protected:
		Unit(BWAPI::Unit bwapiUnit)
			: id(bwapiUnit->getID())
			, bwapiUnit(bwapiUnit)
			, isPlayer(bwapiUnit->getPlayer() == Broodwar->self())
			, squad(nullptr)
			, moveCooldown(0)
			// these are initialized in onFrame()
			, hp_(-1)
			, pos(-1, -1)
			, isMoving(false)
			, groundWeaponCooldown(-1)
		{}
	public:
		virtual void onFrame();
		virtual ~Unit() {}

	public:
		bool isAlive() const { return hp_ > 0; }
		unsigned int hp() const { return hp_; }
		void takeDamage(int damage)
		{
			hp_ = std::max(hp_ - damage, 0);
		}

		virtual vector<shared_ptr<Effect>> getNewEffects() = 0;
		virtual const BWAPI::UnitType& unitType() = 0;
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const = 0;
		virtual Unit* clone() const = 0;
		void simulateOneFrameForward(GameState& state);


		int groundWeaponDamage() const					{ return bwapiUnit->getType().groundWeapon().damageAmount(); }
		int groundWeaponCooldownDefault() const			{ return bwapiUnit->getType().groundWeapon().damageCooldown(); }
		int groundWeaponMinRange() const				{ return bwapiUnit->getType().groundWeapon().minRange(); }
		int groundWeaponMaxRange() const				{ return bwapiUnit->getType().groundWeapon().maxRange(); }
		virtual int groundWeaponDamageOffset() const	{ return 1; }
		virtual int groundWeaponMoveCooldownDefault() const = 0;
		virtual shared_ptr<Effect> attack(const Unit& target) const = 0;
		virtual shared_ptr<Effect> move(const BWAPI::Position& offset) const;

	public:
		static Unit& get(const BWAPI::Unit bwapiUnit)
		{
			static const int key = 87073;
			Unit* unit = static_cast<Unit*>(bwapiUnit->getClientInfo(key));
			if (!unit)
			{
				unit = Unit::create(bwapiUnit);
				bwapiUnit->setClientInfo(unit, key);
			}
			return *unit;
		}
	private:
		static Unit* create(BWAPI::Unit bwapiUnit);
	};
}
