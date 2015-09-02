#include "common/dll.hpp"
#include "common/main.hpp"
#include "common/common.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "search/searchers/scripted.hpp"
#include "search/behaviors/attackclosest.hpp"
#include "search/searchingsquad.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;
	using namespace Bot::Search;

	typedef Scripted<Behaviors::AttackClosest> SearcherType;
	typedef SearchingSquad<SearcherType> SquadType;
	typedef GeneralAllUnitsSingleSquad<SquadType> GeneralType;
	return new Main<GeneralType>;
}
