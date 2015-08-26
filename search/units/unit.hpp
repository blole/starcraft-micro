#pragma once
#include "common/common.hpp"

typedef int id_t;

namespace Bot { namespace Search
{
	class Effect;
	class GameState;

	class Unit
	{
	public:
		const id_t id;
		BWAPI::Position pos;
		int hp;
		bool isMoving;
		bool isAttackFrame;
		bool groundWeaponCooldown;

	public:
		bool isAlive() const { return hp > 0; }
		bool isPlayerUnit() const;
		bool isEnemyUnit() const;
		
		BWAPI::Unit getBwapiUnit() const;
		
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState* state) const = 0;
		virtual Unit* clone() const = 0;

	protected:
		Unit(BWAPI::Unit bwapiUnit, id_t id)
			: id(id)
			, hp(bwapiUnit->getHitPoints())
			, isMoving(false)
			, isAttackFrame(false)
			, groundWeaponCooldown(false)
		{}
		virtual void firstFrameInitToAddAlreadyActiveEffects(GameState* state) = 0;
	public:
		virtual ~Unit() {}

	public:
		static unique_ptr<Unit> create(GameState* state, BWAPI::Unit bwapiUnit, id_t id);
	};

	template <typename Derived>
	class Unit_CRTP : public Unit
	{
	public:
		Unit_CRTP(BWAPI::Unit bwapiUnit, id_t id)
			: Unit(bwapiUnit, id)
		{}

		typedef Unit_CRTP<Derived> Base;

		virtual Unit* clone() const final override
		{
			return new Derived(static_cast<Derived const&>(*this));
		}
	};

	#define class_Unit(Type) class Type : public Unit_CRTP<Type>
}}
