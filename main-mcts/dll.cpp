#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>
#include <BWAPI.h>
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "search/searchingsquad.hpp"
#include "search/selecters/ucb.hpp"
#include "search/simulaters/heuristicwrapper.hpp"
#include "search/actionlisters/branchonunit.hpp"
#include "search/searchers/searchermcts.hpp"
#include "search/backpropagaters/uct.hpp"
#include "search/heuristics/sqrthp_dps.hpp"


extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game) { BWAPI::BroodwarPtr = game; }
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace std;
	using namespace Bot;
	using namespace Bot::Search;

	function<Squad*()> newSquad = []{
		static shared_ptr<Searcher> searcher = make_shared<SearcherMCTS<UCT::NodeUCT>>(
			new ActionListers::BranchOnUnit(),
			new Selecters::UCB<UCT::NodeUCT>(),
			new Simulaters::HeuristicWrapper(new Heuristics::SqrtHp_Dps()),
			new Backpropagaters::UCT<UCT::NodeUCT>());
		return new SearchingSquad(searcher);
	};
	
	shared_ptr<GeneralAllUnitsSingleSquad> general = make_shared<GeneralAllUnitsSingleSquad>(newSquad);

	return new Main(general);
}
