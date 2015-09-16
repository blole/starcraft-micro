#pragma once
#include "common/common.hpp"

namespace Bot
{
	struct Squad;
	class Effect;
	class GameState;

	struct FramedEffect
	{
		const int frame;
		shared_ptr<Effect> effect;
		FramedEffect(int offset, const shared_ptr<Effect>& effect)
			: frame(Broodwar->getFrameCount()+offset)
			, effect(effect)
		{}
	};

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
		bool isAttackFrame;
		bool groundWeaponCooldown;
		list<FramedEffect> activeEffects;

	protected:
		Unit(BWAPI::Unit bwapiUnit)
			: id(bwapiUnit->getID())
			, bwapiUnit(bwapiUnit)
			, isPlayer(bwapiUnit->getPlayer() == Broodwar->self())
			, squad(nullptr)
			, hp_(-1000)
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

		const BWAPI::UnitType& unitType() { return bwapiUnit->getType(); }
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const = 0;
		virtual Unit* clone() const = 0;

	public:
		static const Unit& get(const BWAPI::Unit bwapiUnit)
		{
			return getModifiable(bwapiUnit);
		}
		static Unit& getModifiable(const BWAPI::Unit bwapiUnit)
		{
			static const int key = 87073;
			void* ptr = bwapiUnit->getClientInfo(key);
			if (!ptr)
			{
				ptr = Unit::create(bwapiUnit);
				bwapiUnit->setClientInfo(ptr, key);
			}
			return *static_cast<Unit*>(ptr);
		}
	private:
		static Unit* create(BWAPI::Unit bwapiUnit);
	};

	template <typename Derived, const BWAPI::UnitType& UnitType>
	class Unit_CRTP : public Unit
	{
	public:
		Unit_CRTP(BWAPI::Unit bwapiUnit)
			: Unit(bwapiUnit)
		{}

		virtual Unit* clone() const final override
		{
			return new Derived(static_cast<const Derived&>(*this));
		}
	};
}
