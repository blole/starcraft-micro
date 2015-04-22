#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <map>
#include "search/pendingeffects.hpp"

typedef int id_t;

namespace Bot { namespace Search
{
	template <int,class> class Effect;
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
		GameState(std::vector<BWAPI::Unit> playerUnits, std::vector<BWAPI::Unit> enemyUnits);
		GameState(const GameState* parent, Action* action);
		~GameState();

	public:
		unsigned int getFrame() const { return frame; }
		bool isTerminal();

		void advanceFrames(unsigned int framesToAdvance);
		template <template<int, class> class Eff, int A, class B, typename = std::enable_if<std::is_base_of<Effect<A, B>, Eff<A, B>>::value>::type>
		void queueEffect(int frameOffset, Eff<A, B>* action) {}
		void addEffect(int frameOffset, Action* action);

		std::list<const Unit*> playerUnits() const;
		std::list<const Unit*> enemyUnits() const;
		const std::vector<const Unit*>& getUnits() const;

		const Unit* getUnit(id_t id) const;
		Unit* getUnitModifiable(id_t id);
		BWAPI::Unit getBwapiUnit(id_t id) const;
		BWAPI::Unit getBwapiUnit(const Unit* unit) const;

		std::list<const Unit*> enemyUnitsInRange(BWAPI::Position origin, int maxRange) const;
		std::list<const Unit*> enemyUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const;
		std::list<const Unit*> playerUnitsInRange(BWAPI::Position origin, int maxRange) const;
		std::list<const Unit*> playerUnitsInRange(BWAPI::Position origin, int minRange, int maxRange) const;
	private:
		std::list<const Unit*> unitsInRange(const std::list<const Unit*>& outOf, BWAPI::Position origin, int minRange, int maxRange) const;
	};
}}
