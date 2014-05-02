#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <map>

typedef int id_t;

namespace Bot { namespace Units
{
	class GameState;
	class Action;

	class Unit
	{
	public:
		const id_t id;
		BWAPI::Position pos;
		int hp;
		bool isAttackFrame;
		bool groundWeaponCooldown;

	public:
		bool isAlive() const { return hp > 0; }

		virtual std::list<Action*> const possibleActions(GameState* state) = 0;
		virtual Unit* const clone() = 0;

	protected:
		Unit(GameState* state, BWAPI::Unit bwapiUnit, id_t id);

	public:
		static Unit* create(GameState* state, BWAPI::Unit bwapiUnit, id_t id);
	};

	




	class PendingEffect
	{
		std::vector<std::list<Action*>> effects;
	public:
		std::list<Action*>::iterator getEffects()
		{
			return effects.front().begin();
		}
		void advance()
		{
			if (!effects.empty())
				effects.erase(effects.begin());
		}
	};
}}
