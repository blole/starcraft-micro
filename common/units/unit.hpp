#pragma once
#include "common/common.hpp"

namespace Bot {
	struct Squad;
	namespace Search
{
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
		bool isAttackFrame;
		bool groundWeaponCooldown;

	public:
		bool isAlive() const { return hp_ > 0; }
		unsigned int hp() const { return hp_; }
		void takeDamage(int damage)
		{
			hp_ = std::max(hp_ - damage, 0);
		}
		
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const = 0;
		virtual void firstFrameInitToAddAlreadyActiveEffects(GameState& state) = 0; //TODO: const GameState, return actions
		virtual Unit* clone() const = 0;

	protected:
		Unit(BWAPI::Unit bwapiUnit)
			: id(bwapiUnit->getID())
			, bwapiUnit(bwapiUnit)
			, isPlayer(bwapiUnit->getPlayer() == Broodwar->self())
			, squad(nullptr)
		{
			update();
		}
	public:
		virtual ~Unit() {}

		virtual void update()
		{
			hp_ = bwapiUnit->getHitPoints();
			pos = bwapiUnit->getPosition();
			isMoving = bwapiUnit->isMoving();
			isAttackFrame = bwapiUnit->isAttackFrame();
			groundWeaponCooldown = bwapiUnit->getGroundWeaponCooldown()>0; //TODO: use the int?
		}

	public:
		static Unit& get(BWAPI::Unit bwapiUnit)
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

	template <typename Derived>
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
}}
