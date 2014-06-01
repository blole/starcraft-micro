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
	private:
		static std::vector<BWAPI::Unit> bwapiUnits;
		//std::vector<Unit*> playerUnits;
		//std::vector<Unit*> opponentUnits;
		std::vector<Unit*> units;
		const GameState* parent;
		PendingEffects pendingEffects;
		std::list<Action*> possibleActions;
		std::map<Action*, GameState*> expandedChildren;
		int frame;

	protected:
		GameState(const GameState* parent, Action* action);
		GameState(const GameState* parent);

	public:
		GameState(std::vector<BWAPI::Unit> bwapiUnits);
		~GameState();

		int getFrame() const { return frame; }
		void setPossibleActions(std::list<Action*> possibleActions);
		float defaultPolicy();

		GameState* takeAction(Action* action);
		void addEffect(int frameOffset, Action* action);

		const std::list<Action*> getPossibleActions() const		{return possibleActions;}
		const std::vector<Unit*> getUnits() const				{return units;}
		
		const Unit* getUnit(id_t id) const;
		Unit* getUnitModifiable(id_t id);
		BWAPI::Unit getBwapiUnit(id_t id) const;
		BWAPI::Unit getBwapiUnit(const Unit* unit) const;

		std::list<Unit*> unitsInRange(BWAPI::Position pos, int maxRange) const;
		std::list<Unit*> unitsInRange(BWAPI::Position pos, int minRange, int maxRange) const;
	};
}}
