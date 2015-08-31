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

	function<Squad*()> newSquad = [] {
		static shared_ptr<Searcher> searcher = make_shared<Scripted>(
			make_shared<Behaviors::AttackClosest>());
		return new SearchingSquad(searcher);
	};

	shared_ptr<GeneralAllUnitsSingleSquad> general = make_shared<GeneralAllUnitsSingleSquad>(newSquad);

	return new Main(general);
}
