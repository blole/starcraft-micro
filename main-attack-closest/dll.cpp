#include "common/main.hpp"
#include "common/generals/allunitssinglesquad.hpp"
#include "search/players/scripted.hpp"
#include "search/behaviors/attackclosest.hpp"
#include "search/searchingsquad.hpp"
#include "common/dll.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace Bot::Search;

	typedef Players::Scripted<Behaviors::AttackClosest> PlayerType;
	typedef Squads::Playing<PlayerType> SquadType;
	typedef Generals::AllUnitsSingleSquad<SquadType> GeneralType;
	return new Main<GeneralType>;
}
