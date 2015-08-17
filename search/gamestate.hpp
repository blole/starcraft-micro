#pragma once
#include <BWAPI.h>
#include <vector>
#include <deque>
#include <list>

typedef int id_t;

namespace Bot { namespace Search
{
	class Effect;
	class Unit;

	class GameState
	{
		friend class Unit;
	private:
		static std::vector<BWAPI::Unit> bwapiUnits;
		static int playerUnitCount;
	public:
		static BWAPI::Unit getBwapiUnit(const id_t id) { return bwapiUnits[id]; }
		static bool isPlayerUnit(const id_t id) { return id < GameState::playerUnitCount; }
		
		
		
	private:
		std::vector<Unit*> units;
		std::deque<std::vector<Effect*>> pendingEffects;
		unsigned int frame;

	public:
		GameState(std::vector<BWAPI::Unit> playerUnits, std::vector<BWAPI::Unit> enemyUnits);
		~GameState();

	public:
		unsigned int getFrame() const { return frame; }
		bool isTerminal();

		void advanceFrames(unsigned int framesToAdvance);
		void queueEffect(unsigned int frameOffset, Effect* effect);

		std::list<const Unit*> playerUnits() const;
		std::list<const Unit*> enemyUnits() const;
		const std::vector<const Unit*>& getUnits() const;

		const Unit* getUnit(const id_t id) const;
		Unit* getUnitModifiable(const id_t id);

		std::list<const Unit*> enemyUnitsInRange(BWAPI::Position origin, int maxRange) const;
		std::list<const Unit*> enemyUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const;
		std::list<const Unit*> playerUnitsInRange(BWAPI::Position origin, int maxRange) const;
		std::list<const Unit*> playerUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const;
	private:
		std::list<const Unit*> unitsInRange(const std::list<const Unit*>& outOf, BWAPI::Position origin, int minRange, int maxRange) const;
	};
}}
