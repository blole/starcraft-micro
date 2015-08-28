#pragma once
#include <BWAPI.h>
#include "common/common.hpp"
#include "search/units/unit.hpp"
#include <boost/range.hpp>

typedef int id_t;

namespace Bot { namespace Search
{
	class Effect;

	class GameState final
	{
	private:
		unsigned int frame_;
		const unsigned int playerUnitCount;
		deque<vector<shared_ptr<Effect>>> pendingEffects;

	public:
		const vector<unique_ptr<Unit>> units;

	private:
		GameState(unsigned int frame_, unsigned int playerUnitCount, vector<unique_ptr<Unit>> units);
	public:
		GameState(const vector<BWAPI::Unit>& playerUnits, const vector<BWAPI::Unit>& enemyUnits);
		GameState(const GameState& other);
		~GameState() {}
		
	public:
		sub_range<const vector<unique_ptr<Unit>>> playerUnits() const
		{
			return sub_range<const vector<unique_ptr<Unit>>>(units.begin(), units.begin() + playerUnitCount);
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
		unsigned int frame() const { return frame_; }
		bool isTerminal();

		void advanceFrames(unsigned int framesToAdvance);
		void queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect);
	};
}}
