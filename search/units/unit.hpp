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
		const BWAPI::Unit bwapiUnit;
		const bool isPlayer;
		
		BWAPI::Position pos;
		int hp;
		bool isMoving;
		bool isAttackFrame;
		bool groundWeaponCooldown;

	public:
		bool isAlive() const { return hp > 0; }
		
		virtual vector<shared_ptr<Effect>> possibleActions(const GameState& state) const = 0;
		virtual void firstFrameInitToAddAlreadyActiveEffects(GameState& state) = 0; //TODO: const GameState, return actions
		virtual Unit* clone() const = 0;

	protected:
		Unit(BWAPI::Unit bwapiUnit, id_t id)
			: id(id)
			, bwapiUnit(bwapiUnit)
			, isPlayer(bwapiUnit->getPlayer() == BWAPI::Broodwar->self())
			, hp(bwapiUnit->getHitPoints())
			, isMoving(false)
			, isAttackFrame(false)
			, groundWeaponCooldown(false)
		{}
	public:
		virtual ~Unit() {}

	public:
		static unique_ptr<Unit> create(const GameState& state, BWAPI::Unit bwapiUnit, id_t id);
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
