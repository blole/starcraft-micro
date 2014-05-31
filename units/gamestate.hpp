#pragma once
#include <BWAPI.h>
#include <vector>
#include <list>
#include <map>

typedef int id_t;

namespace Bot { namespace Units
{
	class Action;
	class Unit;

	class Settings
	{
	public:
		static bool branchOnUnit;
	};

	class GameState
	{
	private:
		static std::vector<BWAPI::Unit> bwapiUnits;
		//std::vector<Unit*> playerUnits;
		//std::vector<Unit*> opponentUnits;
		std::vector<Unit*> units;
		GameState* parent;
		std::map<Action*, GameState*> children;
		//PendingEffect pendingEffects;
		std::list<Action*> actions;
		int frame;

	protected:
		GameState(GameState* parent, Action* action);

	public:
		GameState(std::vector<BWAPI::Unit> bwapiUnits);
		~GameState();

		int getFrame() { return frame; }
		const Unit* getUnit(id_t id);
		Unit* getUnitModifiable(id_t id);
		BWAPI::Unit getBwapiUnit(Unit* unit);

		std::list<Action*> possibleActions();
		float defaultPolicy();

		GameState* takeAction(Action* action);
		void addEffect(int frameOffset, Action* action);

		std::list<Unit*> unitsInRange(BWAPI::Position pos, int maxRange);
		std::list<Unit*> unitsInRange(BWAPI::Position pos, int minRange, int maxRange);

	};

	std::vector<Action*> search(std::vector<BWAPI::Unit> bwapiUnits)
	{
		std::vector<Action*> bestActions;
		GameState startingState(bwapiUnits);

		return bestActions;
	}
}}
