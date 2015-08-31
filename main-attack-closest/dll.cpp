#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include "common/main.hpp"
#include "common/common.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "search/searchers/scripted.hpp"
#include "search/behaviors/attackclosest.hpp"
#include "search/searchingsquad.hpp"

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
