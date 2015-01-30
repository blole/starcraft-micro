#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <BWAPI.h>
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "bot/MCTSsquad.hpp"
#include "bot/ABCDsquad.hpp"

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
	std::function<Squad*()> newSquad = []{
		return new ABCDsquad(); //new MCTSsquad()
	};
	General* general = new GeneralAllUnitsSingleSquad(newSquad);

	return new Main(general);
}
