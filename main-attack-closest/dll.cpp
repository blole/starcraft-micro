#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/common.hpp"
#include "common/generals/allunitssinglesquad.hpp"
#include "search/players/scripted.hpp"
#include "search/behaviors/attackclosest.hpp"
#include "search/searchingsquad.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace Bot::Search;

	typedef Players::Scripted<Behaviors::AttackClosest> SearcherType;
	typedef Squads::Searching<SearcherType> SquadType;
	typedef Generals::AllUnitsSingleSquad<SquadType> GeneralType;
	return new Main<GeneralType>;
}
