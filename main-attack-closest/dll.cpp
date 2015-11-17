#include "common/main.hpp"
#include "common/behaviors/attackclosest.hpp"
#include "common/dll.hpp"

extern "C" __declspec(dllexport) BWAPI::AIModule* newAIModule()
{
	using namespace Bot;

	return new ScriptedMain<Behaviors::AttackClosest>;
}
