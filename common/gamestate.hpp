#pragma once
#include "common/common.hpp"
#include "common/units/unit.hpp"

namespace Bot
{
	class Effect;

	class GameState final
	{
	private:
		struct GameStateUnitContainer
		{
			unordered_map<BWAPI::Unit, Unit*> unitMap;
			vector<unique_ptr<Unit>> units;
			vector<Unit*> playerUnits;
			vector<Unit*> enemyUnits;
			GameStateUnitContainer(const vector<const Unit*>& playerUnits, const vector<const Unit*>& enemyUnits);
			GameStateUnitContainer(GameStateUnitContainer&& other)
				: unitMap(other.unitMap)
				, units(std::move(other.units))
				, playerUnits(other.playerUnits)
				, enemyUnits(other.enemyUnits)
			{}
		};
	private:
		unsigned int frame_;
	public:
		deque<vector<shared_ptr<Effect>>> pendingEffects;

	private:
		const unordered_map<BWAPI::Unit, Unit*> unitMap;
	public:
		const vector<unique_ptr<Unit>> units;
		const vector<Unit*> playerUnits;
		const vector<Unit*> enemyUnits;

		bool has(const BWAPI::Unit bwapiUnit) const
		{
			return unitMap.find(bwapiUnit) != unitMap.end();
		}
		Unit& get(const BWAPI::Unit bwapiUnit) const
		{
			return *unitMap.at(bwapiUnit);
		}

	private:
		GameState(GameStateUnitContainer u, unsigned int frame_);
	public:
		GameState(const vector<const Unit*>& playerUnits, const vector<const Unit*>& enemyUnits, unsigned int frame_ = 0);
		GameState(const GameState& other);
		~GameState() {}
		
	public:
		const vector<Unit*>& teamunits(bool player) const
		{
			if (player)
				return playerUnits;
			else
				return enemyUnits;
		}

	public:
		unsigned int frame() const { return frame_; }
		unsigned int frameOffset() const { return frame() - Broodwar->getFrameCount(); }

		void advanceFrame();
		void queueEffect(unsigned int frameOffset, shared_ptr<Effect> effect);
	};
}
