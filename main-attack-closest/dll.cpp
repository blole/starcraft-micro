#define WIN32_LEAN_AND_MEAN	// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>
#include <BWAPI.h>
#include "common/main.hpp"
#include "common/generalallunitssinglesquad.hpp"
#include "behaviors/attackclosest.hpp"

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
	using namespace BehaviorTree;

	function<BehaviorTreeNode*()> unitBrain = []{
		return (new SequentialNode())
			->addChild(new Behaviors::AttackClosest());
	};
	shared_ptr<General> general = make_shared<GeneralAllUnitsSingleSquad>(unitBrain);

	return new Main(general);
}
