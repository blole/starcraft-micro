#pragma once
#include <BWAPI.h>
#include "common/common.hpp"
#include "search/units/unit.hpp"
#include <boost/range.hpp>

typedef int id_t;

namespace Bot { namespace Search
{
	class Effect;

	class GameState
	{
		friend class Unit;
	private:
		static vector<BWAPI::Unit> bwapiUnits;
		static int playerUnitCount;
	public:
		static BWAPI::Unit getBwapiUnit(const id_t id) { return bwapiUnits[id]; }
		static bool isPlayerUnit(const id_t id) { return id < GameState::playerUnitCount; }
		
		
		
	private:
		deque<vector<shared_ptr<Effect>>> pendingEffects;
		unsigned int frame;
	public:
		vector<unique_ptr<Unit>> units;

	public:
		GameState(vector<BWAPI::Unit> playerUnits, vector<BWAPI::Unit> enemyUnits);
		GameState(const GameState& o);
		~GameState() {}
		
	public:
		sub_range<const vector<unique_ptr<Unit>>> playerUnits() const
		{
			return sub_range<const vector<unique_ptr<Unit>>>(units.begin(), units.begin()+playerUnitCount);
		}
		sub_range<const vector<unique_ptr<Unit>>> enemyUnits() const
		{
			return sub_range<const vector<unique_ptr<Unit>>>(units.begin() + playerUnitCount, units.end());
		}
		sub_range<const vector<unique_ptr<Unit>>> teamunits(bool player) const
		{
			if (player)
				return playerUnits();
			else
				return enemyUnits();
		}

	public:
		unsigned int getFrame() const { return frame; }
		bool isTerminal();

		void advanceFrames(unsigned int framesToAdvance);
		void queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect);
	};
}}
