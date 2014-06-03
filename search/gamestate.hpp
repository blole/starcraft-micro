#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <map>
#include "search/pendingeffects.hpp"

typedef int id_t;

namespace Bot { namespace Search
{
	class Action;
	class Unit;

	class GameState
	{
		friend class Unit;
	private:
		static std::vector<BWAPI::Unit> bwapiUnits;
		static int playerUnitCount;

		std::vector<Unit*> units;
		PendingEffects pendingEffects;
		unsigned int frame;

	public:
		GameState(std::vector<BWAPI::Unit> bwapiUnits);
		GameState(const GameState* parent, Action* action);
		~GameState();

	public:
		unsigned int getFrame() const { return frame; }
		bool isTerminal();

		void advanceFrames(unsigned int framesToAdvance);
		void enqueueEffect(int frameOffset, Action* action);

		std::list<const Unit*> playerUnits() const;
		std::list<const Unit*> enemyUnits() const;
		const std::vector<const Unit*>& getUnits() const;

		const Unit* getUnit(id_t id) const;
		Unit* getUnitModifiable(id_t id);
		BWAPI::Unit getBwapiUnit(id_t id) const;
		BWAPI::Unit getBwapiUnit(const Unit* unit) const;

		std::list<Unit*> unitsInRange(BWAPI::Position pos, int maxRange) const;
		std::list<Unit*> unitsInRange(BWAPI::Position pos, int minRange, int maxRange) const;
	};
}}
