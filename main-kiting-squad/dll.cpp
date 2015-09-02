#include "common/dll.hpp"
#include "common/main.hpp"
#include "main-kiting-squad/commander.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	typedef Commander GeneralType;
	GeneralType general;
	return new Main<GeneralType>(general);
}
